#ifndef _LONLIFE_NAT_NAT_H_
#define _LONLIFE_NAT_NAT_H_

#include <functional>
#include <comm_def.h>
#include <nat/nat_key.h>
#include <nat/nat_session.h>
#include <nat/session_manager.h>

namespace nat {

using NATHandler = std::function<void(int, EthernetII&)>;

class nat:noncopyable{
public:
	void nat_out(IPv4Address l, uint32_t client, EthernetII& eth);
	void nat_in(EthernetII& eth);
	void timeout();
	void handler(NATHandler h){
		nat_handler_ = h;
	}
public:
	nat();
	~nat();
private:
	void nat_out_icmp(nat_ptr nat, EthernetII& eth)const{
	}
	template<typename T>
	void nat_out_tcp_udp(nat_ptr nat, EthernetII& eth)const{
		auto pdu = eth.find_pdu<T>();
		pdu->sport(nat->local_port_);
	}
	void convert_in(nat_ptr nat, EthernetII& eth)const;
	template<typename T>
	void convert_in_tcp_udp(nat_ptr nat, EthernetII& eth)const{
		auto pdu = eth.find_pdu<T>();
		pdu->dport(nat->origin_port_);
	}
	void remove_nat(nat_ptr nat){

	}
private:
	detail::session_manager			nat_mgr_;
	NATHandler						nat_handler_;
};

} /* namespace nat */

#endif /* _LONLIFE_NAT_NAT_H_ */
