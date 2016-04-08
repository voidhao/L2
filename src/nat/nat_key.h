#ifndef _LONLIFE_NAT_NAT_KEY_H_
#define _LONLIFE_NAT_NAT_KEY_H_

#include <comm_def.h>

namespace nat {
namespace detail{
struct in_key {
	IPv4Address local_ip_;
	IPv4Address remote_ip_;
	uint16_t local_port_{0};
	uint16_t remote_port_{0};
	union{
		uint16_t proto_{0};
		uint16_t icmp_id_;
	};
	uint16_t icmp_seq_{0};
	bool operator == (const in_key& rhs)const{
		return (memcmp(this, &rhs, sizeof(in_key)) == 0);
	}
	bool operator != (const in_key& rhs)const{
		return (memcmp(this, &rhs, sizeof(in_key)) != 0);
	}
	size_t hash()const{
		uint64_t* i = (uint64_t*) &local_ip_;
		uint64_t* j = (uint64_t*) &local_port_;
		return (*i) | (*j);
	}
};

static_assert(sizeof(in_key) == 16, "size of nat_key should be 16");

struct out_key{
	IPv4Address local_ip_;
	uint16_t	proto_{0};
	uint16_t	local_port_{0};
	bool operator == (const out_key& rhs)const{
		return (memcmp(this, &rhs, sizeof(out_key)) == 0);
	}
	bool operator != (const out_key& rhs)const{
		return (memcmp(this, &rhs, sizeof(out_key)) != 0);
	}
	size_t hash()const{
		return *(size_t*)this;
	}
};

} // namespace detail
} /* namespace nat */


namespace std{

template<>
struct hash<nat::detail::in_key>{
	size_t operator()(const nat::detail::in_key& key)const{
		return key.hash();
	}
};

template<>
struct hash<nat::detail::out_key>{
	size_t operator()(const nat::detail::out_key& key)const{
		return key.hash();
	}
};

} // namespace std;

#endif /* _LONLIFE_NAT_NAT_KEY_H_ */
