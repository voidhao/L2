#include "net.h"
#include <switcher/switcher.h>
#include <netmap/netmap.h>
#include <conf/iface.h>
#include <conf/log.h>
#include <conf/conf.h>
#include <common/limits.h>
using namespace lonlife;

lonlife_net::lonlife_net():
	switcher_{new switcher::switcher}{
}

lonlife_net::~lonlife_net(){

}

bool lonlife_net::init(){
	switcher_->init();
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
	auto b = std::bind(&lonlife_net::netmap_cb, this,  _1, _2);
	netmap::start(b);
    return true;
}

bool lonlife_net::client_pkt(const byte_t* data, size_t size){
	return switcher_->on_client_pkt(data, size);
}
bool lonlife_net::switch_pkt(frame_ptr frame){
	return true;
}
bool lonlife_net::nat_pkt(const byte_t* data, size_t size){
	return true;
}

void lonlife_net::netmap_cb(const byte_t* data, size_t size){
	try{
		auto eth = Tins::EthernetII::extract_metadata(data, size);
		auto type = eth.next_pdu_type;
		if(type != Tins::PDU::IP){
			return;
		}
		auto ip = Tins::IP::extract_metadata(data + eth.header_size, size - eth.header_size);
		if(ip.next_pdu_type == Tins::PDU::UDP){
			auto udp = Tins::UDP(data + eth.header_size + ip.header_size, common::udp_len);
			if(udp.dport() == conf::client_port){
				client_pkt(data, size);
				return;
			}
		}
		switcher_->on_nat_pkt(data, size);
	}catch(std::exception&){
	}
}
