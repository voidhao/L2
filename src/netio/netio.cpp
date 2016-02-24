#define NETMAP_WITH_LIBS
#include <net/netmap_user.h>
#include <stdio.h>
#include <strings.h>
#include "netio.h"
#include "detail/netio_detail.h"


namespace netio{

dev_info_ptr open_dev(const char* dev)
{
    return detail::dev_info_tools::new_dev_info(dev);
}

void close_all_dev(){
    memset(detail::dev_info_reg, 0, sizeof(detail::dev_info_reg));
    for(auto& info : detail::devices){
        detail::dev_info_tools::delete_dev_info(info);
    }
}

void dump_dev(){
//	for(auto& desc : detail::all_open_devices){
//		printf("%s\n", desc->nifp->ni_name);
//	}
}

void wait_pkt(pkt_handler handler){

}

dev_info::~dev_info(){
    if(desc_){
        nm_close(desc_);
    }
}

int dev_info::fd()const{
    return desc_->fd;
}

size_t dev_info::tx_rings_nr()const{
    return desc_->nifp->ni_tx_rings;
}

size_t dev_info::rx_rings_nr()const{
    return desc_->nifp->ni_rx_rings;
}

const char* dev_info::dev_name()const{
    return desc_->nifp->ni_name;
}

bool netio_run(){
    detail::rx_thread = new std::thread(detail::recieve_thread_func);
//    detail::rx_thread->
    return true;
}

void netio_stop(){
    if(detail::rx_thread){
        detail::rx_thread->join();
        delete detail::rx_thread;
        detail::rx_thread = nullptr;
    }
}

rx_pkts wait_rx_pkts(){
    if(!detail::rx_thread){
        fprintf(stderr, "netio thread not run\n");
        rx_pkts pkts;
        return pkts;
    }
   return  detail::recieve_queue.wait();
}
} // namespace netio











