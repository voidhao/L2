#ifndef _LONLIFE_NAT_NAT_H_
#define _LONLIFE_NAT_NAT_H_

#include <comm_def.h>
#include <nat/nat_key.h>
#include <nat/nat_session.h>
#include <nat/session_manager.h>

namespace nat {

class nat:noncopyable{
public:
	void nat_pkt(const byte_t* data, size_t size);
	void nat_out(IPv4Address l, uint32_t client, EthernetII& eth);
	void nat_in(EthernetII& eth);
	void nat_in_icmp(EthernetII& eth);
	void timeout();
public:
	nat();
	~nat();
private:
	void nat_out_icmp(nat_ptr nat, EthernetII& eth){
	}
	template<typename T>
	void nat_out_tcp_udp(nat_ptr nat, EthernetII& eth){
		auto pdu = eth.find_pdu<T>();
		pdu->sport(nat->local_port_);
	}
private:
	detail::session_manager			nat_mgr_;
};

} /* namespace nat */

#endif /* _LONLIFE_NAT_NAT_H_ */
