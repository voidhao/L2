#ifndef _LONLIFE_COMMON_UTIL_H_
#define _LONLIFE_COMMON_UTIL_H_
#include <comm_def.h>

namespace util{

inline uint64_t make_id(IPv4Address ip, uint16_t port){
	return (uint64_t(ip) << 32) | port;
}

}
#endif /* _LONLIFE_COMMON_UTIL_H_ */
