#ifndef _LONLIFE_NAT_SESSION_MANAGER_H_
#define _LONLIFE_NAT_SESSION_MANAGER_H_

#include <comm_def.h>
#include <nat/nat_key.h>

namespace nat {

class nat_session;
using nat_ptr = std::shared_ptr<nat_session>;

namespace detail{

class session_manager:noncopyable {
public:
	session_manager();
	~session_manager();
	nat_ptr find_nat(const out_key& out);
	nat_ptr find_nat(const in_key& in);
	void add_nat(nat_ptr nat);
	void remove_nat(nat_ptr nat);
private:
	using weak			= std::weak_ptr<nat_session>;
	using shared		= std::shared_ptr<nat_session>;
	using out_nat 		= std::unordered_map<detail::out_key, shared>;
	using in_nat	 	= std::unordered_map<detail::in_key, shared>;
	using all_nat 	 	= std::unordered_set<shared>;
	using client_nats	= std::set<shared>;
	using clients_nat 	= std::unordered_map<uint32_t, client_nats>;
private:
	mutable std::mutex	mutex_;
	in_nat				in_nat_;
	out_nat				out_nat_;
	all_nat				all_nat_;
	clients_nat			clients_nat_;
};

} // namespace detail
} /* namespace nat */

#endif /* SESSION_MANAGER_H_ */
