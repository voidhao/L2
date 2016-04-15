#include <nat/nat.h>
#include <common/errors.h>
#include <nat/nat_session.h>
#include <netinet/in.h>
#include <nat/session_manager.h>
#include <nat/nat_key.h>
#include <netmap/netmap.h>
#include <switcher/switcher.h>
namespace nat {

using nat_ptr = std::shared_ptr<nat_session>;

nat::nat(){
}

nat::~nat() {
}

void nat::nat_in(EthernetII& eth){
	auto nat = nat_mgr_.find_in_nat(eth);
	if(!nat){
		return;
	}
	convert_in(nat, eth);
	if(nat->is_tcp_udp()){
		nat->update();
	}else{
		remove_nat(nat);
	}
	switcher_->recv_nat_pkt(nat->client_id_, eth);
}

void nat::nat_out(IPv4Address l, uint32_t client, EthernetII& eth){
	auto ip = eth.find_pdu<IP>();
	if(unlikely(!ip)){
		return;
	}
	auto nat = nat_mgr_.get_out_nat(l, client, eth);
	if(unlikely(!nat)){
		return;
	}
	switch (ip->protocol()){
	case IPPROTO_TCP:
		nat_out_tcp_udp<TCP>(nat, eth);
		break;
	case IPPROTO_UDP:
		nat_out_tcp_udp<UDP>(nat, eth);
		break;
	case IPPROTO_ICMP:
		nat_out_icmp(nat, eth);
		break;
	default:
		return;
	}
	ip->src_addr(nat->local_ip_);
	lonlife::netmap::send(l, eth);
}

void nat::convert_in(nat_ptr nat, EthernetII& eth)const{
	auto ip = eth.find_pdu<IP>();
	if(likely(!ip)){
		return;
	}
	ip->dst_addr(nat->origin_ip_);
	switch (ip->protocol()){
	case IPPROTO_TCP:
		convert_in_tcp_udp<TCP>(nat, eth);
		break;
	case IPPROTO_UDP:
		convert_in_tcp_udp<UDP>(nat, eth);
		break;
	default:
		break;
	}
}

} /* namespace nat */
