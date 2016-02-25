#ifndef _NETIO_HDR_H_
#define _NETIO_HDR_H_

#include <inttypes.h>
#include <memory>
#include <vector>

class nm_desc;

namespace netio{

const int MTU_SIZE = 1500;
const int MAX_DEV  = 1024;
using byte_t = unsigned char;

//forward declares
class dev_info;
class rx_pkt;
class tx_pkt;

using dev_info_ptr  = dev_info*;
using rx_pkt_ptr    = std::shared_ptr<rx_pkt>;
using rx_pkts       = std::vector<rx_pkt_ptr>;
using tx_pkt_ptr    = std::shared_ptr<tx_pkt>;
using tx_pkts       = std::vector<tx_pkt_ptr>;

}
#endif /* HDR_H_ */
