#include "thread.h"
#include <conf/log.h>
namespace net{
namespace detail{

net_thread_t::net_thread_t(){

}

net_thread_t::~net_thread_t(){
    if(thread_){
        thread_.reset();
    }
}

void net_thread_t::start(){
    bool run = false;
    if(run_.compare_exchange_strong(run, true)){
        thread_.reset(new std::thread(&net_thread_t::thread_func, this));
    }
}

void net_thread_t::stop(){
    bool run = true;
    if(run_.compare_exchange_strong(run, false)){
        thread_->join();
    }
}

void net_thread_t::thread_func(){
    zlog_info(sys_log, "net thread start");
    zlog_info(sys_log, "net thread exit");
}

}// namespace net
}// namespace detail


