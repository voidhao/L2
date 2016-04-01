#ifndef _CLIENT_SESSION_H_
#define _CLIENT_SESSION_H_

#include <comm_def.h>
#include <common/util.h>

namespace client{

class session:noncopyable{
public:
	IPv4Address src_ip()const{return src_ip_;}
	IPv4Address dst_ip()const{return dst_ip_;}
	uint16_t	sport()const{return sport_;}
	uint16_t	dport()const{return dport_;}
	uint64_t	conn_id()const{return util::make_id(src_ip_, sport_);}
	uint64_t	uid()const{return uid_;}
	void		src_ip(IPv4Address ip){src_ip_ = ip;}
	void		dst_ip(IPv4Address ip){dst_ip_ = ip;}
	void		sport(uint16_t port){sport_ = port;}
	void		dport(uint16_t port){dport_ = port;}
	void		uid(uint64_t id){uid_ = id;}
	time_t		update_time()const{return update_;}
	void		update(){update_ = time(NULL);}
	bool operator == (const session& rhs)const;
	bool operator != (const session& rhs)const;
	std::atomic_size_t& rx(){return rx_;}
	std::atomic_size_t& rx_size(){return rx_size_;}
	std::atomic_size_t& tx(){return tx_;}
	std::atomic_size_t& tx_size(){return tx_size_;}
private:
	IPv4Address		src_ip_;
	IPv4Address		dst_ip_;
	uint16_t		sport_{0};
	uint16_t		dport_{0};
	uint32_t		priority_{0};
	time_t			login_{time(NULL)};			// 连接时间
	time_t			update_{time(NULL)};		// 最后收到数据时间戳
	uint64_t		uid_;
	std::atomic_size_t rx_{0};					// 上行
	std::atomic_size_t rx_size_{0};
	std::atomic_size_t tx_{0};					// 下行
	std::atomic_size_t tx_size_{0};
};

using session_ptr = std::shared_ptr<session>;
using session_map = std::unordered_map<uint64_t, session_ptr>;

} // namespace client

#endif /* SESSION_H_ */
