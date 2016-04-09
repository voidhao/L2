#include <nat/session_manager.h>
#include <nat/nat_session.h>
#include <netinet/in.h>

namespace nat {
namespace detail{

session_manager::session_manager() {
}

session_manager::~session_manager() {
}

nat::nat_ptr session_manager::find_nat(const out_key& out){
	lock_guard lock(mutex_);
	auto it = out_nat_.find(out);
	if(it == out_nat_.end()){
		return nullptr;
	}
	return it->second;
}
nat::nat_ptr session_manager::find_nat(const in_key& in){
	lock_guard lock(mutex_);
	auto it = in_nat_.find(in);
	if(it == in_nat_.end()){
		return nullptr;
	}
	return it->second;
}
void session_manager::add_nat(nat_ptr nat){
	auto out_key = out_key_from_nat(nat);
	auto in_key = in_key_from_nat(nat);
	auto client = nat->client_id_;
	lock_guard lock(mutex_);
	in_nat_.insert(std::pair<struct in_key, nat_ptr>(in_key, nat));
	out_nat_.insert(std::pair<struct out_key, nat_ptr>(out_key, nat));
	all_nat_.insert(nat);
	if((nat->proto_ == IPPROTO_UDP) ||
			(nat->proto_ == IPPROTO_TCP)){
		clients_nat_[client].insert(nat);
	}

}

void session_manager::remove_nat(nat_ptr nat){
	auto out_key = out_key_from_nat(nat);
	auto in_key = in_key_from_nat(nat);
	auto client = nat->client_id_;

	lock_guard lock(mutex_);
	in_nat_.erase(in_key);
	out_nat_.erase(out_key);
	all_nat_.erase(nat);
	auto it = clients_nat_.find(client);
	if(it != clients_nat_.end()){
		auto& nats = it->second;
		nats.erase(nat);
	}

}

} // namespace detail
} /* namespace nat */
