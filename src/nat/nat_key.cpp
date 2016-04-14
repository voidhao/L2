#include <nat/nat_key.h>
#include <nat/nat_session.h>
#include <netinet/in.h>
namespace nat{
namespace detail{

nat_key make_out_key(uint32_t client, const IP& ip){
	nat_key ret;
	ret.client_ = client;
	ret.src_ip_ = ip.src_addr();
	ret.dst_ip_ = ip.dst_addr();
	ret.proto_ = ip.protocol();
	if(ip.protocol() == IPPROTO_TCP){
		auto tcp = ip.find_pdu<TCP>();
		if(likely(tcp)){
			ret.sport_ = tcp->sport();
			ret.dport_ = tcp->dport();
		}
	}else if( ip.protocol() == IPPROTO_UDP){
		auto udp = ip.find_pdu<UDP>();
		if(likely(udp)){
			ret.sport_ = udp->sport();
			ret.dport_ = udp->dport();
		}
	}
	return ret;
}

bool make_in_key(nat_key& key, const EthernetII& eth){
	auto ip = eth.find_pdu<IP>();
	if(unlikely(!ip)){
		return false;
	}
	auto proto = ip->protocol();
	switch (proto){
	case IPPROTO_TCP:
		return make_in_key_tcp(key, eth);
	case IPPROTO_UDP:
		return make_in_key_udp(key, eth);
	case IPPROTO_ICMP:
		return make_in_key_icmp(key, eth);
	}
	return false;
}

bool make_in_key_icmp_unreachable(nat_key& key, const EthernetII& eth){
	auto icmp = eth.find_pdu<Tins::ICMP>();
	Tins::RawPDU* pdu = (Tins::RawPDU*) icmp->inner_pdu();
	if(unlikely(!pdu)){
		return false;
	}
	auto data = pdu->serialize();

	try {
		auto ip = Tins::IP(data.data(), data.size());
		if (ip.protocol() == IPPROTO_TCP) {
			auto tcp = ip.find_pdu<TCP>();
			if(unlikely(!tcp)){
				return false;
			}
			key.src_ip_ = ip.src_addr();
			key.dst_ip_ = ip.dst_addr();
			key.sport_ = tcp->sport();
			key.dport_ = tcp->dport();
			key.proto_ = IPPROTO_TCP;
		} else if (ip.protocol() == IPPROTO_UDP) {
			auto udp = ip.find_pdu<UDP>();
			if(unlikely(!udp)){
				return false;
			}
			key.src_ip_ = ip.src_addr();
			key.dst_ip_ = ip.dst_addr();
			key.sport_ = udp->sport();
			key.dport_ = udp->dport();
			key.proto_ = IPPROTO_UDP;
		}
	} catch (std::exception& e) {
		return false;
	}
	return true;
}
bool make_in_key_icmp_reply(nat_key& key, const EthernetII& eth){
	auto ip = eth.find_pdu<IP>();
	auto icmp = eth.find_pdu<Tins::ICMP>();
	if(unlikely((!ip) || (!icmp))){
		return false;
	}
	key.src_ip_ = ip->dst_addr();
	key.dst_ip_ = ip->src_addr();
	key.proto_ = IPPROTO_ICMP;
	key.icmp_id_ = icmp->id();
	key.icmp_seq_ = icmp->sequence();
	return true;
}

bool make_in_key_udp(nat_key& key, const EthernetII& eth){
	auto ip = eth.find_pdu<IP>();
	auto udp = eth.find_pdu<Tins::UDP>();
	if(unlikely((!ip) || (!udp))){
		return false;
	}
	key.src_ip_ = ip->dst_addr();
	key.dst_ip_ = ip->src_addr();
	key.sport_ = udp->dport();
	key.dport_ = udp->sport();
	key.proto_ = IPPROTO_UDP;
	return true;
}

bool make_in_key_tcp(nat_key& key, const EthernetII& eth){
	auto ip = eth.find_pdu<IP>();
	auto tcp = eth.find_pdu<Tins::TCP>();
	if(unlikely((!ip) || (!tcp))){
		return false;
	}
	key.src_ip_ = ip->dst_addr();
	key.dst_ip_ = ip->src_addr();
	key.sport_ = tcp->dport();
	key.dport_ = tcp->sport();
	key.proto_ = IPPROTO_TCP;
	return true;
}

bool make_in_key_icmp(nat_key& key, const EthernetII& eth){
	auto ip = eth.find_pdu<IP>();
	if(unlikely(!ip)){
		return false;
	}
	auto icmp = ip->find_pdu<Tins::ICMP>();
	if(unlikely(!icmp)){
		return false;
	}

	auto type = icmp->type();
	if(type == Tins::ICMP::DEST_UNREACHABLE){
		return make_in_key_icmp_unreachable(key, eth);
	}else{
		return make_in_key_icmp_reply(key, eth);
	}
}

bool make_out_key(nat_key& key, uint32_t client, const EthernetII& eth){
	auto ip = eth.find_pdu<IP>();
	if(unlikely(!ip)){
		return false;
	}
	key.client_ = client;
	key.src_ip_ = ip->src_addr();
	key.dst_ip_ = ip->dst_addr();
	switch (ip->protocol()){
	case IPPROTO_TCP:
		return make_out_tcp_udp<TCP>(key, eth);
	case IPPROTO_UDP:
		return make_out_tcp_udp<UDP>(key, eth);
	default:
		return false;
	}
}

bool make_out_icmp(nat_key& key, const EthernetII& eth){
	auto ip = eth.find_pdu<IP>();
	auto icmp  = eth.find_pdu<ICMP>();
	if(unlikely(!ip || !icmp)){
		return false;
	}
	key.src_ip_ = ip->src_addr();
	key.dst_ip_ = ip->dst_addr();
	key.icmp_id_ = icmp->id();
	key.icmp_seq_ = icmp->sequence();
	key.proto_ = IPPROTO_ICMP;
	return true;
}

nat_key out_key_from_nat(nat_ptr nat){
	nat_key key;
	key.client_ = nat->client_id_;
	key.proto_ = nat->proto_;
	key.src_ip_ = nat->origin_ip_;
	key.dst_ip_ = nat->remote_ip_;
	switch(key.proto_){
	case IPPROTO_TCP:
	case IPPROTO_UDP:
		key.sport_ = nat->origin_port_;
		key.dport_ = nat->remote_port_;
		break;
	}
	return key;
}
nat_key in_key_from_nat(nat_ptr nat){
	nat_key key;
	key.proto_ = nat->proto_;
	key.src_ip_ = nat->local_ip_;
	key.dst_ip_ = nat->remote_ip_;
	switch (key.proto_){
	case IPPROTO_TCP:
	case IPPROTO_UDP:
		key.sport_ = nat->local_port_;
		key.dport_ = nat->remote_port_;
		break;
	case IPPROTO_ICMP:
		key.icmp_id_ = nat->icmp_id_;
		key.icmp_seq_ = nat->icmp_seq_;
		break;
	}
	return key;
}

} // namespace detail
} // namespace nat


