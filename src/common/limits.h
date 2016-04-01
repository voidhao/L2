#ifndef _LONLIFE_COMMON_LIMITS_H_
#define _LONLIFE_COMMON_LIMITS_H_

namespace common{

	const size_t eth_len = 14;
	const size_t ip_len  = 20;
	const size_t udp_len = 8;
	const size_t tcp_len = 20;
	const size_t vxlan_len = 8;
	const size_t outer_len = eth_len + ip_len + udp_len + vxlan_len;
	const size_t inner_len = eth_len + ip_len;

}
#endif /* LIMITS_H_ */
