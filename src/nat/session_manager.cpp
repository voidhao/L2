#include <nat/session_manager.h>
#include <nat/nat_session.h>
#include <netinet/in.h>

namespace nat {
namespace detail{

session_manager::session_manager() {
}

session_manager::~session_manager() {
}

nat::nat_ptr session_manager::find_out_nat(const nat_key& key){
	lock_guard lock(mutex_);
	auto it = out_nat_.find(key);
	if(it == out_nat_.end()){
		return nullptr;
	}
	return it->second;
}
nat::nat_ptr session_manager::find_in_nat(const nat_key& key){
	lock_guard lock(mutex_);
	auto it = in_nat_.find(key);
	if(it == in_nat_.end()){
		return nullptr;
	}
	return it->second;
}
void session_manager::add_nat(nat_ptr nat){
	auto out_key = out_key_from_nat(nat);
	auto in_key = in_key_from_nat(nat);
	auto client = nat->client_id_;
	lock_guard lock(mutex_);
	in_nat_.insert(std::pair<struct nat_key, nat_ptr>(in_key, nat));
	clients_nat_[client].insert(nat);
	all_nats_.insert(nat);
	if(nat->is_tcp_udp()){
		out_nat_.insert(std::pair<struct nat_key, nat_ptr>(out_key, nat));
	}
}

nat_ptr session_manager::get_out_nat(IPv4Address l,
										uint32_t client,
										const EthernetII& eth){
	if(eth.find_pdu<TCP>() || eth.find_pdu<UDP>()){
		return get_out_nat_tcp_udp(l, client, eth);
	}
	if(eth.find_pdu<ICMP>()){
		return get_out_nat_icmp(l, client, eth);
	}
	return nullptr;
}

nat_ptr session_manager::get_out_nat_tcp_udp(IPv4Address l, uint32_t client, const EthernetII& eth){
	nat_key key;
	if(!make_out_key(key, client, eth)){
		return nullptr;
	}
	auto nat = find_out_nat(key);
	if(!nat){
		nat = make_nat_session(l , client, eth);
	}
	if(!nat){
		return nullptr;
	}
	add_nat(nat);
	return nat;
}

nat_ptr session_manager::get_out_nat_icmp(IPv4Address l, uint32_t client, const EthernetII& eth){
	auto nat = make_nat_session(l , client, eth);
	if(unlikely(!nat)){
		return nullptr;
	}
	add_nat(nat);
	return nat;
}
nat_ptr session_manager::find_in_nat(const EthernetII& eth){
	nat_key key;
	if(likely(detail::make_in_key(key, eth))){
		return find_in_nat(key);
	}
	return nullptr;
}

nat_ptr session_manager::make_nat_session(IPv4Address l,
										uint32_t client,
										const EthernetII& eth){
	auto ip = eth.find_pdu<IP>();
	if(unlikely(!ip)){
		return nullptr;
	}

	nat_ptr nat;
	int proto = ip->protocol();
	switch (proto){
	case IPPROTO_TCP:
		nat =  fill_origin_tcp_udp<TCP>(eth);
		break;
	case IPPROTO_UDP:
		nat =  fill_origin_tcp_udp<UDP>(eth);
		break;
	case IPPROTO_ICMP:
		nat = fill_origin_icmp(eth);
		break;
	default:
		nat = nullptr;
	}
	if(unlikely(!nat)){
		return nat;
	}

	nat->client_id_ = client;
	nat->local_ip_ = l;
	return nat;
}

} // namespace detail
} /* namespace nat */
