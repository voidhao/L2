#include "client.h"

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

} // namespace client


