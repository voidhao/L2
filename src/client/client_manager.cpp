#include "session.h"
#include "client_manager.h"

namespace client{

client_manager mangaer;
void client_manager::add_session(session_ptr sess){
	auto conn_id = sess->conn_id();
	auto uid = sess->uid();
	lock_guard lock(session_mutex_);
	conn_to_session_[conn_id] = sess;
	uid_to_session_[uid] = sess;
}

session_ptr client_manager::find_session(uint64_t uid)const{
	lock_guard lock(session_mutex_);
	auto it = uid_to_session_.find(uid);
	if(it != uid_to_session_.end()){
		return it->second;
	}else{
		return nullptr;
	}
}

session_ptr client_manager::find_session(IPv4Address ip, uint16_t port)const{
	auto id = util::make_id(ip, port);
	lock_guard lock(session_mutex_);
	auto it = conn_to_session_.find(id);
	if(it != conn_to_session_.end()){
		return it->second;
	}else{
		return nullptr;
	}
}

void client_manager::remove_session(session_ptr sess){
	auto uid = sess->uid();
	auto conn_id = sess->conn_id();
	lock_guard lock(session_mutex_);
	uid_to_session_.erase(uid);
	conn_to_session_.erase(conn_id);
}

void client_manager::remove_session(IPv4Address ip, uint16_t port){
	auto conn_id = util::make_id(ip, port);
	lock_guard lock(session_mutex_);
	auto it = conn_to_session_.find(conn_id);
	if(it != conn_to_session_.end()){
		auto uid = it->second->uid();
		uid_to_session_.erase(uid);
		conn_to_session_.erase(conn_id);
	}
}

void client_manager::remove_session(uint64_t uid){
	lock_guard lock(session_mutex_);
	auto it = uid_to_session_.find(uid);
	if(it != uid_to_session_.end()){
		auto conn_id = it->second->conn_id();
		uid_to_session_.erase(uid);
		conn_to_session_.erase(conn_id);
	}
}

}



