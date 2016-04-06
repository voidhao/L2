#include "net.h"
#include <switcher/switcher.h>
#include <netmap/netmap.hpp>

using namespace lonlife;

lonlife_net::lonlife_net():
	switcher_{new switcher::switcher}{
}

lonlife_net::~lonlife_net(){

}

bool lonlife_net::init(){
	return lonlife::netmap::init();
}

bool lonlife_net::start(){
    netmap::open_dev("eth1", IPv4Address("10.0.0.1"));
    netmap::listen(IPv4Address("0.0.0.0"),
    		IPv4Address("0.0.0.0"),
    		IPPROTO_UDP, 4789,
    		std::bind(&lonlife_net::client_pkt, this, _1));
    netmap::start();
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
