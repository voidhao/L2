#ifndef _LONLIFE_NAT_SESSION_MANAGER_H_
#define _LONLIFE_NAT_SESSION_MANAGER_H_

#include <comm_def.h>
#include <nat/nat_key.h>
#include <nat/port_manager.h>
#include <nat/nat_session.h>
#include <netinet/in.h>
namespace nat {

class nat_session;
using nat_ptr = std::shared_ptr<nat_session>;

namespace detail{

class session_manager:noncopyable {
public:
	session_manager();
	~session_manager();

	void remove_nat(nat_ptr nat);
	void remvoe_client_all_nats(uint32_t client);
	nat_ptr get_out_nat(IPv4Address l, uint32_t client, const EthernetII& eth);
	nat_ptr find_in_nat(const EthernetII& eth);
private:
	using client_nats	= std::set<nat_ptr>;
	using clients_nat 	= std::unordered_map<uint32_t, client_nats>;
	using nat_map		= std::unordered_map<detail::nat_key, nat_ptr>;
	using all_nats		= std::unordered_set<nat_ptr>;

private:
	nat_ptr find_out_nat(const nat_key& key);
	nat_ptr find_in_nat(const nat_key& key);
	nat_ptr get_out_nat_tcp_udp(IPv4Address l, uint32_t client, const EthernetII& eth);
	nat_ptr get_out_nat_icmp(IPv4Address l, uint32_t client, const EthernetII& eth);
	nat_ptr make_nat_session(IPv4Address l, uint32_t client, const EthernetII& eth);
	void add_nat(nat_ptr nat);
	template<typename T>
	nat_ptr fill_origin_tcp_udp(const EthernetII& eth){
		auto ip = eth.find_pdu<IP>();
		auto pdu = ip->find_pdu<T>();
		if(unlikely(!ip && !pdu)){
			return nullptr;
		}
		auto nat = std::make_shared<nat_session>();
		nat->origin_ip_ = ip->src_addr();
		nat->remote_ip_ = ip->dst_addr();
		nat->origin_port_ = pdu->sport();
		nat->remote_port_ = pdu->dport();
		nat->proto_ = ip->protocol();
		nat->local_port_ = get_port<T>(port_mgr_);
		return nat;
	}
	nat_ptr fill_origin_icmp(const EthernetII& eth){
		auto ip = eth.find_pdu<IP>();
		auto icmp = ip->find_pdu<ICMP>();
		if(unlikely(!ip || !icmp)){
			return nullptr;
		}
		auto nat = std::make_shared<nat_session>();
		nat->origin_ip_ = ip->src_addr();
		nat->remote_ip_ = ip->dst_addr();
		nat->icmp_id_ = icmp->id();
		nat->icmp_seq_ = icmp->sequence();
		nat->proto_ = IPPROTO_ICMP;
		return nat;
	}
private:
	mutable std::mutex	mutex_;
	nat_map				in_nat_;
	nat_map				out_nat_;
	all_nats			all_nats_;
	clients_nat			clients_nat_;
	port_manager		port_mgr_;
};

} // namespace detail
} /* namespace nat */

#endif /* SESSION_MANAGER_H_ */
