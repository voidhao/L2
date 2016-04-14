#include <nat/nat.h>
#include <common/errors.h>
#include <nat/nat_session.h>
#include <netinet/in.h>
#include <nat/session_manager.h>
#include <nat/nat_key.h>
#include <netmap/netmap.h>
//#include <netmap/netmap.hpp>
namespace nat {

using nat_ptr = std::shared_ptr<nat_session>;

nat::nat(){
}

nat::~nat() {
}

void nat::nat_in(EthernetII& eth){
	auto ip = eth.find_pdu<IP>();
	if(unlikely(!ip)){
		return;
	}
	auto proto = ip->protocol();
	switch (proto){
	case IPPROTO_TCP:
		break;
	case IPPROTO_UDP:
		break;
	case IPPROTO_ICMP:
		nat_in_icmp(eth);
	default:
		return;
	}
}

void nat::nat_in_icmp(EthernetII& eth){
	auto ip = eth.find_pdu<IP>();
	auto icmp = ip->find_pdu<ICMP>();
	if(unlikely(!ip || !icmp)){
		return;
	}
}

//void nat::on_icmp(EthernetII& eth){
//	detail::in_key in_key;
//	auto icmp = eth.find_pdu<Tins::ICMP>();
//	if (icmp->type() == Tins::ICMP::DEST_UNREACHABLE) {
//		Tins::RawPDU* pdu = (Tins::RawPDU*) icmp->inner_pdu();
//		CHECK_PDU(pdu);
//		auto data = pdu->serialize();
//		try {
//			auto ip = Tins::IP(data.data(), data.size());
//			if (ip.protocol() == IPPROTO_TCP) {
//				auto tcp = ip.find_pdu<TCP>();
//				CHECK_PDU(tcp);
//				in_key.local_ip_ = ip.src_addr();
//				in_key.remote_ip_ = ip.dst_addr();
//				in_key.local_port_ = tcp->sport();
//				in_key.remote_port_ = tcp->dport();
//				in_key.proto_ = IPPROTO_TCP;
//			} else if (ip.protocol() == IPPROTO_UDP) {
//				auto udp = ip.find_pdu<Tins::UDP>();
//				CHECK_PDU(udp);
//				in_key.local_ip_ = ip.src_addr();
//				in_key.remote_ip_ = ip.dst_addr();
//				in_key.local_port_ = udp->sport();
//				in_key.remote_port_ = udp->dport();
//				in_key.proto_ = IPPROTO_UDP;
//			}
//		} catch (std::exception& e) {
//			return;
//		}
//
//	} else {
//		auto ip = eth.find_pdu<IP>();
//		CHECK_PDU(ip);
//		in_key.local_ip_ = ip->dst_addr();
//		in_key.remote_ip_ = ip->src_addr();
//		in_key.icmp_id_ = icmp->id();
//		in_key.icmp_seq_ = icmp->sequence();
//	}
//
//	auto nat = nat_mgr_.find_nat(in_key);
//	if (!nat) {
//		return;
//	}
//}

void nat::nat_out(IPv4Address l, uint32_t client, EthernetII& eth){
	auto ip = eth.find_pdu<IP>();
	if(unlikely(!ip)){
		return;
	}
	ip->dst_addr("10.0.0.105");
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
	default:
		return;
	}
	ip->src_addr(nat->local_ip_);
	lonlife::netmap::send(l, eth);
}

} /* namespace nat */
