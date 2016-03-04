#include <sys/ioctl.h>
#include <poll.h>
#include <string.h>
#include "../pkt.h"
#include "../netio.h"
#include "netio_detail.h"
#include "send.h"

namespace netio{
namespace detail{


pkt_sender_t::pkt_sender_t(){

}

pkt_sender_t::~pkt_sender_t(){

}

bool pkt_sender_t::push(dev_info_ptr dev, tx_pkt_ptr pkt){
    std::unique_lock<std::mutex> lock(mutex_);
    if (dev->tx_pkts_buff_.size() < TX_BUFF_SIZE){
        dev->tx_pkts_buff_.push_back(pkt);
        cond_.notify_one();
        lock.unlock();
        return true;
    } else {
        lock.unlock();
        stastics_drops(dev, 1);
        return false;
    }
}

dev_tx_queues pkt_sender_t::wait(){
    dev_tx_queues queues;
    std::unique_lock<std::mutex> lock(mutex_);
    while(run_){
        for(auto& dev : devices){
            if(!dev->tx_pkts_buff_.empty()){
                auto queue = std::make_pair(dev, std::move(dev->tx_pkts_buff_));
                queues.push_back(std::move(queue));
            }
        }

        if (queues.empty()){
            cond_.wait(lock);
            continue;
        }else{
            break;
        }
    }
    return std::move(queues);
}

void pkt_sender_t::start(){
    bool run = false;
    if(run_.compare_exchange_strong(run, true)){
        thread_.reset(new std::thread(&pkt_sender_t::thread_func, this));
    }
}

void pkt_sender_t::stop(){
    run_ = false;
    cond_.notify_all();
    if(thread_){
        thread_->join();
        thread_.reset();
    }
}

void pkt_sender_t::thread_func(){
    printf("tx thread start\n");
    for(auto& dev : devices){
        int fd = dev->fd();
        ioctl(fd, NIOCTXSYNC);
    }

    while(run_){
        auto queues = wait();
        for(auto& queue : queues){
            send_pkts_to_netmap(queue);
        }
    }
    printf("tx thread exit\n");
}

void pkt_sender_t::send_pkts_to_netmap(dev_tx_queue const& queue){
    dev_info_ptr dev = queue.first;
    nm_desc* desc = dev->desc();
    auto& pkts = queue.second;
    size_t total_pkts_nr = pkts.size();
    if(total_pkts_nr == 0){
        return;
    }
    bool stop_send = false;
    pollfd fd;
    fd.fd = desc->fd;
    fd.events = POLLOUT;
    size_t send_nr = 0;
    size_t send_size = 0;
    size_t total_ring_nr = desc->last_tx_ring - desc->first_tx_ring + 1;
    do{
        int n = poll(&fd, 1, 5);
        if(n < 0 && errno == EINTR){
            continue;
        }
        if (n < 0){
            dev->conn_error();
            return;
        }
        if (n == 0){
            size_t drops = (total_pkts_nr - send_nr);
            stastics_drops(dev, drops);
            return;
        }
        dev->conn_recover();

        for(size_t i = 0; i < total_ring_nr &&  !stop_send; i++){
            netmap_ring* ring = nullptr;
            uint32_t ri = desc->cur_tx_ring + i;
            uint32_t current_slot = 0;
            uint32_t idx = 0;
            if(ri > desc->last_tx_ring){
                ri -= total_ring_nr;
            }
            ring = NETMAP_TXRING(desc->nifp, ri);
            while(!nm_ring_empty(ring) && !stop_send){
                    auto& pkt = pkts[send_nr];
                    size_t pkt_size = pkt->size();
                    current_slot = ring->cur;
                    idx = ring->slot[current_slot].buf_idx;
                    char* buff = NETMAP_BUF(ring, idx);
                    memcpy(buff, pkt->data(), pkt_size);
                    ring->slot[current_slot].len =  pkt_size;
                    ring->head = ring->cur;
                    ring->cur = nm_ring_next(ring, current_slot);
                    send_nr++;
                    send_size += pkt_size;
                    if(!(send_nr < total_pkts_nr)){
                        stop_send = true;
                    }
                    bool empty = nm_ring_empty(ring);
                    if(empty){
                        break;
                    }
            }
        }
    }while(!stop_send);

    int err = ioctl(desc->fd, NIOCTXSYNC, 0);
    if(err != 0){
        perror("send pkt ioctl\n");
    }
    stastics_tx(dev, send_nr, send_size);
}

void pkt_sender_t::stastics_tx(dev_info_ptr dev, size_t nr, size_t size){
    tx_nr_+= nr;
    tx_size_ += size;
    dev->tx_nr_ += nr;
    dev->tx_size_ += size;
}

void pkt_sender_t::stastics_drops(dev_info_ptr dev, size_t nr){
    tx_drops_nr_ += nr;
    dev->tx_drops_nr_ += nr;
}

}} // namespace

