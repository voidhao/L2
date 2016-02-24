#include <poll.h>
#include <strings.h>
#include <stdio.h>
#define NETMAP_WITH_LIBS
#include <net/netmap_user.h>
#include "netio_detail.h"


namespace netio{
namespace detail{

std::vector<dev_info_ptr>   devices;
dev_info_ptr                dev_info_reg[MAX_DEV];
recieve_queue_t             recieve_queue;
std::thread*                rx_thread = nullptr;
std::thread*                tx_thread = nullptr;

void recieve_thread_func(){
    printf("netmap tx thread run\n");
    std::vector<pollfd> poll_fds;
    for(auto& dev : devices){
        pollfd fd;
        fd.fd = dev->fd();
        fd.events = POLLIN;
        fd.revents = 0;
        poll_fds.push_back(fd);
    }
    for(;;){
        int n = poll(poll_fds.data(), poll_fds.size(), 0);
        if(n == -1){
            perror("netmap poll");
            break;
        }
        for(auto& fd : poll_fds){
            if(fd.revents & POLLIN){
                dev_info* dev = dev_info_reg[fd.fd];
                recieve_pkt_from_netmap(dev);
            }
        }
    }
    printf("netmap tx thread exit\n");
}

void recieve_pkt_from_netmap(dev_info* dev){

}
dev_info* dev_info_tools::new_dev_info(nm_desc* desc){
    dev_info* info = nullptr;
    if(desc){
        info = new dev_info(desc);
    }
    return info;
}

dev_info* dev_info_tools::new_dev_info(const char* dev){
    dev_info* info = nullptr;

    //check dev opened
    for(auto& info : devices){
        int cmp = strncmp(dev, info->dev_name(), IFNAMSIZ);
        if(cmp == 0){
            return dev_info_reg[info->fd()];
        }
    }

    char buff[256]={0};
    snprintf(buff, 256, "netmap:%s", dev);
    nm_desc* desc = nm_open(buff, nullptr, 0, 0);
    if(!desc){
        return info;
    }
    if (desc->fd >= MAX_DEV){
        fprintf(stderr, "netmap open dev fd too large:%d\n", desc->fd);
        nm_close(desc);
        return info;
    }

    info = new dev_info(desc);

    devices.push_back(info);
    dev_info_reg[desc->fd] = info;

    return info;
}



void dev_info_tools::delete_dev_info(dev_info* info){
    if(info){
        delete info;
    }
}

void recieve_queue_t::stop(){
	std::lock_guard<std::mutex> lock(mutex_);
	run_ = false;
	cond_.notify_all();
}

rx_pkts recieve_queue_t::wait(){
    std::unique_lock<std::mutex> lock(mutex_);
    run_ = true;
    while(pkts_.empty() && run_){
        cond_.wait(lock);
    }
    return std::move(pkts_);
}

void recieve_queue_t::push(const rx_pkts& pkts){
    std::unique_lock<std::mutex> lock(mutex_);
    if (!run_){
        return;
    }
    if(pkts_.empty()){
        pkts_ = std::move(pkts);
        cond_.notify_one();
        return;
    } else {
        size_t left, more;
        left = pkts_.size();
        more = pkts.size();
        for(auto& pkt : pkts){
            pkts_.push_back(pkt);
        }
        cond_.notify_one();
        lock.unlock();
        printf("process rx pkt too slow, %ld left and %ld more\n", left, more );
        return;
    }
}

}} // namespace
