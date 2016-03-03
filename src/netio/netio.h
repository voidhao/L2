#ifndef _NETIO_NETIO_H_
#define _NETIO_NETIO_H_


#include "hdr.h"
#include "dev_info.h"
#include "pkt.h"

namespace netio{

dev_info_ptr  open_dev(const char*);
bool          netio_run();
void          netio_stop();
rx_pkts       wait_rx_pkts();
void          send_pkt(dev_info_ptr dev, char const* data, size_t size);
size_t        rx_nr();
size_t        rx_size();
size_t        rx_drops();
size_t        tx_nr();
size_t        tx_size();
size_t        tx_drops();

} // namespace netio
#endif /* _NETIO_NETIO_H_ */
