#include <nat/nat_key.h>
#include <nat/nat_session.h>
#include <netinet/in.h>
namespace nat{
namespace detail{

struct in_key in_key_from_nat(nat_ptr nat){
	struct in_key in_key;
	switch (nat->proto_){
	case IPPROTO_TCP:
	case IPPROTO_UDP:
		in_key.local_port_ = nat->local_port_;
		in_key.remote_port_ = nat->remote_port_;
		in_key.proto_ = nat->proto_;
		break;
	case IPPROTO_ICMP:
		in_key.icmp_id_ = nat->icmp_id_;
		in_key.icmp_seq_ = nat->icmp_seq_;
		break;
	}
	return in_key;
}
struct out_key out_key_from_nat(nat_ptr nat){
	struct out_key out_key;
	out_key.client_id_ = nat->client_id_;
	out_key.origin_ip_ = nat->origin_ip_;
	out_key.origin_port_ = nat->origin_port_;
	out_key.proto_ = nat->proto_;
	return out_key;
}

struct out_key out_key_from_ip(uint32_t client, const IP&ip){
	struct out_key out_key;
	out_key.client_id_ = client;
	out_key.origin_ip_ = ip.src_addr();
	out_key.proto_ = ip.protocol();
	if(out_key.proto_ == IPPROTO_TCP){
		auto tcp = ip.find_pdu<TCP>();
		if(likely(tcp)){
			out_key.origin_port_ = tcp->sport();
		}
	}else if( out_key.proto_ == IPPROTO_UDP){
		auto udp = ip.find_pdu<UDP>();
		if(likely(udp)){
			out_key.origin_port_ = udp->sport();
		}
	}
	return out_key;
}

} // namespace detail
} // namespace nat


