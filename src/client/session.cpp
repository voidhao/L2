#include "client.h"
#include <netmap/netmap.h>
namespace client{

bool session::operator == (const session& rhs)const{
	return 	(src_ip_ == rhs.src_ip_)&&
			(dst_ip_ == rhs.dst_ip_ )&&
			(sport_ == rhs.sport_)&&
			(dport_ == rhs.dport_);
}

bool session::operator != (const session& rhs)const{
	return 	(src_ip_ != rhs.src_ip_)||
			(sport_ != rhs.sport_)||
			(dst_ip_ != rhs.dst_ip_ )||
			(dport_ != rhs.dport_);
}

size_t session::send_to_client(EthernetII& inner){
	auto eth = EthernetII()/IP(IPv4Address("10.0.0.27"), IPv4Address("10.0.0.167"))/UDP(sport_, 4789)/Tins::RawPDU((const uint8_t*)&meta_, sizeof(meta_)) / inner;
	return lonlife::netmap::send(IPv4Address("10.0.0.167"), eth);
}
} // namespace client


