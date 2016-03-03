#include <poll.h>
#define NETMAP_WITH_LIBS
#include <net/netmap_user.h>
#include "../netio.h"
#include "../pkt.h"
#include "netio_detail.h"
#include "reciever.h"

namespace netio{
namespace detail{


pkt_reciever_t::pkt_reciever_t(){

}

pkt_reciever_t::~pkt_reciever_t(){

}

void pkt_reciever_t::push(rx_pkt_ptr pkt){
    std::lock_guard<std::mutex> lock(mutex_);
    if(pkts_.size() < RX_BUFF_SIZE){
        pkts_.push_back(pkt);
        cond_.notify_one();
        stastics_rx(pkt->dev(), 1, pkt->size());
    } else {
        stastics_drops(pkt->dev(), 1);
    }
}

void pkt_reciever_t::push(rx_pkts& pkts, dev_info_ptr dev, size_t size){
    size_t nr = pkts.size();
    std::unique_lock<std::mutex> lock(mutex_);
    if(pkts_.empty()){
        pkts_ = std::move(pkts);
        lock.unlock();
        cond_.notify_one();
        stastics_rx(dev, nr, size);
        return;
    }
    size_t rx_nr = 0;
    size_t rx_drops = 0;
    size_t rx_size = 0;
    for(size_t i = 0; i < nr && pkts_.size() < RX_BUFF_SIZE; i++){
        auto& pkt = pkts[i];
        pkts_.push_back(pkt);

        rx_nr++;
        rx_size += pkt->size();

    }
    lock.unlock();
    cond_.notify_one();
    rx_drops = nr - rx_nr;
    stastics_rx(dev, rx_nr, rx_size);
    stastics_drops(dev, rx_drops);
}

void pkt_reciever_t::start(){
    bool run = false;
    if(run_.compare_exchange_strong(run, true)){
        thread_.reset(new std::thread(&pkt_reciever_t::thread_func, this));
    }
}

void pkt_reciever_t::stop(){
    run_ = false;
    cond_.notify_all();
    if(thread_){
        thread_->join();
        thread_.reset();
    }
}
rx_pkts pkt_reciever_t::wait(){
    std::unique_lock<std::mutex> lock(mutex_);
    while(run_){
        if(pkts_.empty()){
            cond_.wait(lock);
            continue;
        }
        break;
    }
    return std::move(pkts_);
}

void pkt_reciever_t::thread_func(){
    printf("rx thread run\n");
    std::vector<pollfd> poll_fds;
    for(auto& dev : devices){
        pollfd fd;
        fd.fd = dev->fd();
        fd.events = POLLIN;
        fd.revents = 0;
        poll_fds.push_back(fd);
    }

    while(run_){
        int n = poll(poll_fds.data(), poll_fds.size(), 3000);
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
                dev_info_ptr dev = dev_info_reg[fd.fd];
                recieve(dev);
            }
        }
    }
    printf("rx thread exit\n");
}

void pkt_reciever_t::recieve(dev_info_ptr dev){
    rx_pkts pkts;
    size_t rx_size = 0;
    nm_desc* desc = dev->desc();
    int ring_index = desc->cur_rx_ring;
    do{
        netmap_ring* ring = NETMAP_RXRING(desc->nifp, ring_index);
        while(!nm_ring_empty(ring)){
            uint32_t i = ring->cur;
            uint32_t idx = ring->slot[i].buf_idx;
            byte_t*  buff = (byte_t*)NETMAP_BUF(ring, idx);
            uint32_t size = ring->slot[i].len;
            rx_size += size;
            rx_pkt_ptr pkt = std::make_shared<rx_pkt>(dev, buff, size);
            pkts.push_back(pkt);
            ring->cur = nm_ring_next(ring, i);
            ring->head = ring->cur;
        }
        ring_index++;
        if(ring_index > desc->last_rx_ring){
            ring_index = desc->first_rx_ring;
        }
    }while(ring_index != desc->cur_rx_ring);

    if(pkts.size() == 1){
        push(pkts[0]);
    }else if(pkts.size() > 1){
        push(pkts, dev, rx_size);
    }
}

void pkt_reciever_t::stastics_rx(dev_info_ptr dev, size_t nr, size_t size){
    rx_nr_ += nr;
    rx_size_ += size;
    dev->rx_nr_ += nr;
    dev->rx_size_ += size;
}

void pkt_reciever_t::stastics_drops(dev_info_ptr dev, size_t nr){
    rx_drops_nr_  += nr;
    dev->rx_drops_nr_ += nr;
}

}} // namespace
