#ifndef _LONLIFE_CLIENT_CLIENT_MANAGER_H_
#define _LONLIFE_CLIENT_CLIENT_MANAGER_H_

#include <comm_def.h>
#include <common/util.h>
#include <switcher/tuple.h>
namespace client{

class session;
using session_ptr = std::shared_ptr<session>;
using session_map = std::unordered_map<uint64_t, session_ptr>;

class client_manager:noncopyable{
public:
	void			add_session(session_ptr sess);
	session_ptr		find_session(IPv4Address ip, uint16_t port)const;
	session_ptr		find_session(uint64_t uid)const;
	void remove_session(session_ptr sess);
	void remove_session(IPv4Address ip, uint16_t port);
	void remove_session(uint64_t uid);
	template<typename Tuple>
	session_ptr client_tuple(const Tuple& tuple){
		auto& outer = tuple.outer_;
		auto& meta = tuple.meta_;
		auto sess = find_session(outer.src_addr(), outer.sport());
		if(!sess){
			sess =  make_session(outer.src_addr(), outer.sport(), meta.uid(), meta);
		}
		sess->update();
		return sess;
	}
private:
	session_ptr make_session(IPv4Address src, uint16_t port, uint64_t uid, const switcher::vxlan_meta& meta);
	mutable std::mutex	session_mutex_;
	session_map			conn_to_session_;
	session_map			uid_to_session_;
};

extern client_manager mangaer;
}
#endif /* _LONLIFE_CLIENT_CLIENT_MANAGER_H_ */
