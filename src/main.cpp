

#include <unistd.h>
#include <netmap/netmap.hpp>
#include "init.h"

using namespace lonlife;
struct handler{
	bool handle(netmap::frame_ptr frame){
		auto ip = frame->find_pdu<Tins::IP>();
		if(ip){
//			std::cout << "cap from " << ip->src_addr() << std::endl;
		}
		return true;
	}
};

int main(int argc, char** argv) {
	handler h;
    init();
    lonlife::netmap::init();
    lonlife::netmap::open_dev("eth0", Tins::IPv4Address("10.0.0.1"), lonlife::netmap::default_open_flags);
    netmap::listen(netmap::IPv4Address(),
    		netmap::IPv4Address(), IPPROTO_IP, 0, std::bind(&handler::handle, &h, std::placeholders::_1));
    lonlife::netmap::start();
    sleep(1000);
    lonlife::netmap::stop();
    return 0;
}
