#ifndef _LONLIFE_NAT_NAT_SESSION_H_
#define _LONLIFE_NAT_NAT_SESSION_H_

#include <comm_def.h>


namespace client{
class session;
}

namespace nat {

class nat_session {
private:
	using clt_ptr = std::weak_ptr<client::session>;
public:
	nat_session();
	~nat_session();
private:
	IPv4Address	remote_ip_;
	IPv4Address	local_ip_;
	int16_t		remote_port_{0};
	int16_t		local_port_{0};
	int32_t		proto_{0};
	int			path_{0};
	clt_ptr		client_;
	time_t		last_{time(NULL)};
};

} /* namespace nat */

#endif /* _LONLIFE_NAT_NAT_SESSION_H_ */
