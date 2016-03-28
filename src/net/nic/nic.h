#ifndef _NET_NIC_H_
#define _NET_NIC_H_

#include <comm_def.h>
#include <lonlife/netmap.h>
#include <net/nic/ip_addr.h>

namespace net{
namespace nic{

const int IFNAME_SIZE = 16;
const int MAC_LEN = 6;
const int MAC_STR_LEN = 32;

using dev_info_ptr = lonlife::netmap::dev_info_ptr;

const_nic_ptr       find_nic(const char* name);
const_nic_ip_ptr    find_nic_ip(uint32_t ip);
void                handle_arp_pkt(const Tins::PDU& pdu);
bool                init();
bool                update_nic();
bool                update_nic_ip();
void                nic_opened(const char* name, dev_info_ptr dev);

} // namespace nic
} // namespace
#endif /* _NET_NIC_H_ */
