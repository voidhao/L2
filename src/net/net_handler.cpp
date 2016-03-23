#include <net/net_handler.h>
#include <conf/iface.h>
#include <lonlife/netmap.h>
#include <conf/log.h>
#include <conf/conf.h>
#include <net/detail/pkt.h>

namespace net {

net_handler_t net_handler;

net_handler_t::net_handler_t() {
}

net_handler_t::~net_handler_t() {
}

void net_handler_t::thread_func(){
    zlog_info(sys_log, "pkt handler thread start");
    while(run_){
        auto pkts = lonlife::netmap::wait_rx_pkts();
        for(auto& pkt : pkts){
            pkts_handler(pkt);
        }
    }
    zlog_info(sys_log, "pkt handler thread exit");
}

void net_handler_t::start(){

    nics_ = conf::read_iface();
    if(nics_.empty()){
        zlog_error(sys_log, "interace conf is empty");
        return;
    }

    for(auto& nic : nics_){
        lonlife::netmap::open_dev(nic.c_str());
    }

    lonlife::netmap::netio_run();

    bool run = false;
    if(run_.compare_exchange_strong(run, true)){
        thread_.reset(new std::thread(&net_handler_t::thread_func, this));
    }
}

void net_handler_t::stop(){
    bool run = true;
    if(run_.compare_exchange_strong(run, false)){
        lonlife::netmap::netio_stop();
        thread_->join();
        thread_.reset();
    }
}

bool net_handler_t::pkts_handler(lonlife::netmap::rx_pkt_ptr& pkt){
    if(arp_handler(pkt)){
        return true;
    }
    return true;
}

bool net_handler_t::arp_handler(lonlife::netmap::rx_pkt_ptr& pkt){
    return false;
}

bool net_handler_t::client_handler(lonlife::netmap::rx_pkt_ptr& pkt){
    const byte_t* data = pkt->data();
    size_t size = pkt->size();
    if(detail::is_udp(data, size) &&
            (detail::udp_dest_port(data, size) == conf::client_port)){
        return true;
    } else {
        return false;
    }
}

} /* namespace net */
