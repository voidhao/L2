#ifndef _LONLIFE_NAT_NAT_H_
#define _LONLIFE_NAT_NAT_H_

#include <comm_def.h>
#include <nat/nat_key.h>
#include <nat/nat_session.h>
#include <nat/session_manager.h>
#include <nat/port_manager.h>

namespace nat {

class nat:noncopyable{
public:
	error_t nat_client_pkt(uint32_t client_id, uint32_t path, IP& eth);
	void timeout();
public:
	nat();
	~nat();
private:
	nat_ptr get_nat_ptr(uint32_t client, uint32_t path, IP& ip);
	nat_ptr	make_nat_ptr(uint32_t client, uint32_t path, const IP& ip);
	void fill_ports(nat_ptr nat, const IP& ip);
private:
	detail::port_manager			port_mgr_;
	detail::session_manager			nat_mgr_;
};

namespace detail{
	extern std::atomic_ushort ip_id;
	inline uint16_t get_ip_id(){
		return  ip_id++;
	}
}

} /* namespace nat */

#endif /* _LONLIFE_NAT_NAT_H_ */
