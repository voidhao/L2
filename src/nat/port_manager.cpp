/*
 * port_manager.cpp
 *
 *  Created on: Apr 9, 2016
 *      Author: vincent
 */

#include <nat/port_manager.h>

namespace nat {
namespace detail{
port_manager::port_manager() {
	// TODO Auto-generated constructor stub

}

port_manager::~port_manager() {
	// TODO Auto-generated destructor stub
}


void port_manager::init_ports(){
	for(uint16_t i = 0; i < 20000; i++){
		uint16_t port = i + 20000;
		free_udp_ports_.insert(port);
		free_tcp_ports_.insert(port);
	}
}

uint16_t port_manager::get_port(ports_set& free_ports, ports_set& inuse_ports){
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

uint16_t port_manager::get_udp_port(){
	return get_port(free_udp_ports_, inuse_udp_ports_);
}
uint16_t port_manager::get_tcp_port(){
	return get_port(free_tcp_ports_, inuse_tcp_ports_);
}

} // namespace detail
} /* namespace nat */
