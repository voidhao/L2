#ifndef _LONLIFE_NAT_NAT_KEY_H_
#define _LONLIFE_NAT_NAT_KEY_H_

#include <comm_def.h>
#include <netinet/in.h>

namespace nat {

class nat_session;
using nat_ptr = std::shared_ptr<nat_session>;

namespace detail{

struct nat_key{
	IPv4Address	src_ip_;
	IPv4Address dst_ip_;
	union{
		uint16_t	sport_{0};
		uint16_t	icmp_id_;
	};
	union{
		uint16_t	dport_{0};
		uint16_t	icmp_seq_;
	};
	uint32_t	client_{0};
	uint8_t		proto_{0};


	bool operator == (const nat_key& rhs)const{
		return (memcmp(this, &rhs, sizeof(nat_key)) == 0);
	}

	bool operator != (const nat_key& rhs)const{
		return (memcmp(this, &rhs, sizeof(nat_key)) != 0);
	}

	size_t hash()const{
		size_t* v1 = (size_t*)&src_ip_;
		size_t* v2 = (size_t*)&sport_;
		return (*v1)|(*v2);
	}

	bool is_tcp_udp()const{
		return (proto_ == IPPROTO_UDP) || (proto_ == IPPROTO_TCP);
	}
	bool is_icmp()const{
		return (proto_ == IPPROTO_ICMP);
	}
};



using key_ptr = std::shared_ptr<nat_key>;
nat_key make_out_key(uint32_t client, const IP& ip);

bool make_out_key(nat_key& key, uint32_t client, const EthernetII& eth);
bool make_out_icmp(nat_key& key, const EthernetII& eth);
template<typename T>
bool make_out_tcp_udp(nat_key& key, const EthernetII& eth);
bool make_in_key(nat_key& key, const EthernetII& eth);
bool make_in_key_icmp(nat_key& key, const EthernetII& eth);
bool make_in_key_icmp_unreachable(nat_key& key, const EthernetII& eth);
bool make_in_key_icmp_reply(nat_key& key, const EthernetII& eth);
bool make_in_key_udp(nat_key& key, const EthernetII& eth);
bool make_in_key_tcp(nat_key& key, const EthernetII& eth);

nat_key out_key_from_nat(nat_ptr nat);
nat_key in_key_from_nat(nat_ptr nat);

template<typename T>
bool make_out_tcp_udp(nat_key& key, const EthernetII& eth){
	auto ip = eth.find_pdu<IP>();
	auto pdu = eth.find_pdu<T>();
	if(unlikely(!ip || !pdu)){
		return false;
	}
	key.src_ip_ = ip->src_addr();
	key.dst_ip_ = ip->dst_addr();
	key.sport_ = pdu->sport();
	key.dport_ = pdu->dport();
	key.proto_ = ip->protocol();
	return true;
}

} // namespace detail
} /* namespace nat */


namespace std{

template<>
struct hash<nat::detail::key_ptr>{
	size_t operator()(const nat::detail::key_ptr& key)const{
		return key->hash();
	}
};

template<>
struct equal_to<nat::detail::key_ptr>{
	bool operator()(const nat::detail::key_ptr& lhs, const nat::detail::key_ptr& rhs)const{
		return (*lhs) == (*rhs);
	}
};

template<>
struct hash<nat::detail::nat_key>{
	size_t operator()(const nat::detail::nat_key& key)const{
		return key.hash();
	}
};

template<>
struct equal_to<nat::detail::nat_key>{
	bool operator()(const nat::detail::nat_key& lhs, const nat::detail::nat_key& rhs)const{
		return lhs == rhs;
	}
};

} // namespace std;

#endif /* _LONLIFE_NAT_NAT_KEY_H_ */
