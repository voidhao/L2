#include <nat/nat.h>
#include <common/errors.h>
namespace nat {

nat::nat() {
	// TODO Auto-generated constructor stub
	init_ports();
}

nat::~nat() {
	// TODO Auto-generated destructor stub
}

error_t nat::nat_pkt(clt_ptr sess, IP& ip){
	return err::SUCCESS;
}

void nat::init_ports(){
	for(uint16_t i = 0; i < 20000; i++){
		uint16_t port = i + 20000;
		free_udp_ports_.insert(port);
		free_tcp_ports_.insert(port);
	}
}

uint16_t nat::get_udp_port(){
	return get_port(free_udp_ports_, inuse_udp_ports_);
}

uint16_t nat::get_tcp_port(){
	return get_port(free_tcp_ports_, inuse_tcp_ports_);
}

uint16_t nat::get_port(ports_set& free_ports, ports_set& inuse_ports){
	lock_guard lock(port_mutex_);
	if(!free_ports.empty()){
		auto it = free_ports.begin();
		auto port = *it;
		free_ports.erase(it);
		inuse_ports.insert(port);
		return port;
	}
	return 0;
}

void nat::free_port(uint16_t port, ports_set& free_ports, ports_set& inuse_ports){
	lock_guard lock(port_mutex_);
	free_ports.insert(port);
	inuse_ports.erase(port);
}

} /* namespace nat */
