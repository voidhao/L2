#ifndef _LONLIFE_CLIENT_CLIENT_MANAGER_H_
#define _LONLIFE_CLIENT_CLIENT_MANAGER_H_

#include <comm_def.h>

namespace client{

class session;
using session_ptr = std::shared_ptr<session>;
using session_map = std::unordered_map<uint64_t, session_ptr>;

class client_manager:noncopyable{
public:
	void			add_session(session_ptr sess);
	session_ptr		find_session(IPv4Address ip, uint16_t port)const;
	session_ptr		find_session(uint64_t uid)const;
	void	remove_session(session_ptr sess);
	void	remove_session(IPv4Address ip, uint16_t port);
	void	remove_session(uint64_t uid);
private:
	mutable std::mutex	session_mutex_;
	session_map			conn_to_session_;
	session_map			uid_to_session_;
};

extern client_manager mangaer;
}
#endif /* _LONLIFE_CLIENT_CLIENT_MANAGER_H_ */
