#ifndef _NETIO_HDR_H_
#define _NETIO_HDR_H_

#include <inttypes.h>
#include <sys/time.h>
#include <string.h>
#include <strings.h>
#include <utility>
#include <memory>
#include <vector>

namespace netio{

const int MTU_SIZE = 1500;
const int MAX_DEV  = 1024;
using byte_t = unsigned char;

//forward declares

class dev_info;
class rx_pkt;

using rx_pkt_ptr = rx_pkt*;
using rx_pkts    = std::vector<rx_pkt_ptr>;

}
#endif /* HDR_H_ */
