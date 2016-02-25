#include "netio_detail.h"

namespace netio{
namespace detail{

void send_queue_t::push(tx_pkt_ptr pkt){
    std::lock_guard<std::mutex> lock(mutex_);
    if(!run_){
        return;
    }
    pkts_.push_back(pkt);
    cond_.notify_one();
}

tx_pkts send_queue_t::wait(){
    std::unique_lock<std::mutex> lock(mutex_);
    run_ = true;
    while(pkts_.empty() && run_){
        cond_.wait(lock);
    }
    return std::move(pkts_);
}

void send_thread_func(){
    printf("netmap send thread run\n");
    for(;;){
        tx_pkts pkts = send_queue.wait();
        if(pkts.empty()){
            continue;
        }
    }
    printf("netmap send thread exit\n");
}

}} // namespace

