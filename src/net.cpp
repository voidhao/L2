#include "net.h"
#include <switcher/switcher.h>
#include <netmap/netmap.h>
#include <conf/iface.h>
#include <conf/log.h>
using namespace lonlife;

void cb(const unsigned char* data, size_t size){

}
lonlife_net::lonlife_net():
	switcher_{new switcher::switcher}{
}

lonlife_net::~lonlife_net(){

}

bool lonlife_net::init(){
	return lonlife::netmap::init();
}

bool lonlife_net::start(){
	for(auto& iface : conf::read_ifaces()){
		auto nic = netmap::open(iface.name_.c_str());
		if(!nic){
			zlog_error(sys_log, "open %s faild", iface.name_.c_str());
		} else {
			zlog_info(sys_log, "open %s success", iface.name_.c_str());
		}
		netmap::nic_gateway(nic, iface.gateway_);
	}
	auto b = std::bind(cb, _1, _2);
	netmap::start(b);
    return true;
}

bool lonlife_net::client_pkt(frame_ptr frame){
	auto data = frame->serialize();
	return switcher_->on_client_pkt(data.data(), data.size());
}
bool lonlife_net::switch_pkt(frame_ptr frame){
	return true;
}
bool lonlife_net::nat_pkt(const byte_t* data, size_t size){
	return true;
}
