#ifndef _LONLIFE_NAT_NAT_H_
#define _LONLIFE_NAT_NAT_H_

#include <comm_def.h>
#include <nat/nat_key.h>
namespace client{
class session;
}

namespace nat {

class nat_session;
class out_key;
class in_key;

class nat:noncopyable{
private:
	using clt_ptr = std::weak_ptr<client::session>;
public:
	error_t nat_pkt(int path, IP& ip);
	error_t nat_pkt(clt_ptr sess, IP&ip);
	size_t	free_tcp_ports_count()const{
		lock_guard lock(port_mutex_);
		return free_tcp_ports_.size();
	}
	size_t free_udp_ports_count()const{
		lock_guard lock(port_mutex_);
		return free_udp_ports_.size();
	}
	void timeout();
public:
	nat();
	~nat();
private:
	using ports_set = std::unordered_set<uint16_t>;
	using out_map = std::unordered_map<detail::out_key, std::shared_ptr<nat_session>>;
	using in_map = std::unordered_map<detail::in_key, std::shared_ptr<nat_session>>;
	void init_ports();
	uint16_t get_udp_port();
	uint16_t get_tcp_port();
	uint16_t get_port(ports_set& free_ports, ports_set& inuse_ports);
	void free_port(uint16_t port, ports_set& free_ports, ports_set& inuse_ports);
	error_t	do_udp_nat();
private:
	mutable std::mutex	port_mutex_;
	ports_set			free_tcp_ports_;
	ports_set			inuse_tcp_ports_;
	ports_set			free_udp_ports_;
	ports_set			inuse_udp_ports_;
	mutable std::mutex	sess_mutex_;
	in_map				in_map_;
	out_map				out_map_;
};

} /* namespace nat */

#endif /* _LONLIFE_NAT_NAT_H_ */
