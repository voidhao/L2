#include <poll.h>
#define NETMAP_WITH_LIBS
#include <net/netmap_user.h>
#include "../netio.h"
#include "netio_detail.h"

namespace netio{
namespace detail{

rx_pkts recieve_queue_t::wait(){
    std::unique_lock<std::mutex> lock(mutex_);
    run_ = true;
    while(pkts_.empty() && run_){
        cond_.wait(lock);
    }
    return std::move(pkts_);
}

void recieve_queue_t::push(rx_pkts const& pkts){
    std::unique_lock<std::mutex> lock(mutex_);
    if (!run_){
        return;
    }
    if(pkts_.empty()){
        pkts_ = std::move(pkts);
        cond_.notify_one();
        lock.unlock();
        imm_pkt_nr_ += pkts.size();
        return;
    } else {
        for(auto& pkt : pkts){
            pkts_.push_back(pkt);
        }
        cond_.notify_one();
        lock.unlock();
        delay_pkt_nr_ += pkts.size();
        return;
    }
}

void recieve_thread_func(){
    printf("netmap recieve thread run\n");
    std::vector<pollfd> poll_fds;
    for(auto& dev : devices){
        pollfd fd;
        fd.fd = dev->fd();
        fd.events = POLLIN;
        fd.revents = 0;
        poll_fds.push_back(fd);
    }
    for(;;){
        int n = poll(poll_fds.data(), poll_fds.size(), -1);
        if(n == -1 && errno == EINTR){
            continue;
        }
        if(n == -1){
            perror("netmap rx thread poll");
            break;
        }
        if(n == 0){
            continue;
        }
        for(auto& fd : poll_fds){
            if(fd.revents & POLLIN){
                dev_info* dev = dev_info_reg[fd.fd];
                rx_pkts pkts = recieve_pkts_from_netmap(dev);
                recieve_queue.push(pkts);
            }
        }
    }
    printf("netmap recieve thread exit\n");
}

rx_pkts recieve_pkts_from_netmap(dev_info* dev){
    rx_pkts pkts;
    nm_desc* desc = dev->desc();
    int ring_index = desc->cur_rx_ring;
    do{
        netmap_ring* ring = NETMAP_RXRING(desc->nifp, ring_index);
        while(!nm_ring_empty(ring)){
            uint32_t i = ring->cur;
            uint32_t idx = ring->slot[i].buf_idx;
            byte_t*  buff = (byte_t*)NETMAP_BUF(ring, idx);
            uint32_t size = ring->slot[i].len;
            rx_pkts_nr++;
            rx_pkts_size += size;
            rx_pkt_ptr pkt = std::make_shared<rx_pkt>(dev, buff, size);

            pkt->stamp(ring->ts);
            pkts.push_back(pkt);

            ring->cur = nm_ring_next(ring, i);
            ring->head = ring->cur;
        }

        ring_index++;
        if(ring_index > desc->last_rx_ring){
            ring_index = desc->first_rx_ring;
        }
    }while(ring_index != desc->cur_rx_ring);
    return std::move(pkts);
}

}} // namespace
