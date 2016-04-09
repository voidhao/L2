#ifndef _LONLIFE_NAT_NAT_SESSION_H_
#define _LONLIFE_NAT_NAT_SESSION_H_

#include <comm_def.h>

namespace client{
class session;
}

namespace nat {

struct nat_session :noncopyable{
	IPv4Address	remote_ip_;
	IPv4Address	local_ip_;
	IPv4Address origin_ip_;
	int16_t		remote_port_{0};
	int16_t		local_port_{0};
	int16_t		origin_port_{0};
	int16_t		proto_{0};
	int32_t		path_{0};
	int32_t		client_id_{0};
	uint16_t	icmp_id_{0};
	uint16_t	icmp_seq_{0};
	time_t		last_{time(NULL)};
	time_t		remove_{0};
};

using nat_ptr = std::shared_ptr<nat_session>;

} /* namespace nat */

#endif /* _LONLIFE_NAT_NAT_SESSION_H_ */
