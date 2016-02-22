#ifndef _NETIO_NETIO_H_
#define _NETIO_NETIO_H_

#include <functional>
#include <inttypes.h>
#include <sys/time.h>
#include "iface.h"

namespace netio{

struct pkt{
	uint32_t dev_;
	const char* data_;
	uint32_t size_;
	timeval stamp_;
};

using pkt_handler = std::function<void(pkt*)>;

const int MTU = 1500;


int open_dev(const char*);
bool netio_run();
void netio_stop();
void close_all_dev();
void wait_pkt(pkt_handler handler);
void dump_dev();
} // namespace netio
#endif /* _NETIO_NETIO_H_ */
