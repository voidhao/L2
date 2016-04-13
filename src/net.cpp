#include "net.h"
#include <switcher/switcher.h>
//#include <netmap/netmap.hpp>
#include <n2/netmap.h>
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
//    netmap::open_dev("eth1", IPv4Address("10.0.0.105"));
//    netmap::listen(IPv4Address("0.0.0.0"),
//    		IPv4Address("0.0.0.0"),
//    		IPPROTO_UDP, 4789,
//    		std::bind(&lonlife_net::client_pkt, this, _1));
//    netmap::start();
	auto nic = netmap::open("eth1");
	netmap::nic_gateway(nic, IPv4Address("10.0.0.1"));
	auto b = std::bind(cb, _1, _2);
	auto eth = Tins::EthernetII()/Tins::IP("10.0.0.105", "10.0.0.167")/Tins::RawPDU("Hello, World");
	netmap::send(IPv4Address("10.0.0.167"), eth);
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
