#ifndef _LONLIFE_NAT_PORT_MANAGER_H_
#define _LONLIFE_NAT_PORT_MANAGER_H_

#include <comm_def.h>

namespace nat {
namespace detail{
class port_manager {
public:
	port_manager();
	~port_manager();
	uint16_t get_udp_port();
	uint16_t get_tcp_port();
	void free_tcp_port();
	void free_udp_port();
private:
	using ports_set = std::unordered_set<uint16_t>;
	void init_ports();
	uint16_t get_port(ports_set& free_ports, ports_set& inuse_ports);
private:

	mutable std::mutex	port_mutex_;
	ports_set			free_tcp_ports_;
	ports_set			inuse_tcp_ports_;
	ports_set			free_udp_ports_;
	ports_set			inuse_udp_ports_;
};

template<typename T>
struct get_port_t;
template<>
struct get_port_t<TCP>{
	uint16_t operator()(port_manager& m)const{
		return m.get_tcp_port();
	}
};
template<>
struct get_port_t<UDP>{
	uint16_t operator()(port_manager& m)const{
		return m.get_udp_port();
	}
};
template<typename T>
uint16_t get_port(port_manager& m){
	get_port_t<T> mgr;
	return mgr(m);
}

} // namespace detail
} /* namespace nat */

#endif /* PORT_MANAGER_H_ */
