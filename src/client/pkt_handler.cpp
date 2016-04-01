#include "client.h"
#include <client/client_manager.h>
#include <common/limits.h>
namespace client{

bool client_pkt_handler(frame_ptr frame){
	printf("hello, client %u\n", frame->size());
	outer_header outer;
	fill_outer_header(frame, outer);



	return true;
}

bool fill_outer_header(frame_ptr frame, outer_header& outer){
	IP* ip;
	UDP* udp;
	ip = frame->find_pdu<IP>();
	ip->id();
	CHECK_NULL(ip);
	udp = ip->find_pdu<UDP>();
	CHECK_NULL(udp);
	auto dest = ip->dst_addr();
	auto src = ip->src_addr();
	auto dport = udp->dport();
	auto sport = udp->sport();
	auto session = client::mangaer.find_session(src, sport);
	if(!session){
		session = 	std::make_shared<client::session>();
		session->src_ip(src);
		session->dst_ip(dest);
		session->sport(sport);
		session->dport(dport);
	}

	auto data = frame->serialize();
	Tins::EthernetII inner(data.data() + common::outer_len, data.size() - common::outer_len);
//	inner.extract_metadata(data.data() + common::outer_len, data.size() - common::outer_len);
	auto eth2 = inner.find_pdu<Tins::EthernetII>();
	auto inner_ip = eth2->find_pdu<IP>();
	CHECK_NULL(inner_ip);
	auto extra = inner_ip->extract_metadata(data.data() + common::outer_len + 14, 28);
	auto icmp = inner_ip->find_pdu<Tins::ICMP>();
	CHECK_NULL(icmp);

	dest = inner_ip->dst_addr();
	src = inner_ip->src_addr();
	std::cout << "inner src:" << src << std::endl;
	std::cout << "inner dst:" << dest << std::endl;

	return true;
}

} // namesapce client



