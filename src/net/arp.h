#ifndef _NET_ARP_ARP_H_
#define _NET_ARP_ARP_H_

#include <lonlife/net/addr.h>

namespace net{

    lonlife::net::mac_addr arp(uint32_t);
    lonlife::net::ip4_addr rarp(uint64_t);
    void handle_arp(const char* data, size_t size);

} // namespace net

#endif /* _NET_ARP_ARP_H_ */
