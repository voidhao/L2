#include <poll.h>
#include <strings.h>
#include <stdio.h>
#include <error.h>

#include "netio_detail.h"
#include "../netio.h"


namespace netio{
namespace detail{

std::vector<dev_info_ptr>   devices;
dev_info_ptr                dev_info_reg[MAX_DEV];
recieve_queue_t             recieve_queue;
send_queue_t                send_queue;
std::thread*                rx_thread = nullptr;
std::thread*                tx_thread = nullptr;
std::atomic_size_t          rx_pkts_nr{0};
std::atomic_size_t          rx_pkts_size{0};
std::atomic_size_t          tx_pkts_nr{0};
std::atomic_size_t          tx_pkts_size{0};


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



}} // namespace
