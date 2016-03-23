#ifndef _NET_DETAIL_PKT_H_
#define _NET_DETAIL_PKT_H_

namespace net{
namespace detail{

bool is_udp(const byte_t* data, size_t size);
bool is_tdp(const byte_t* data, size_t size);
bool is_arp(const byte_t* data, size_t size);

uint32_t src_ip(const byte_t* data, size_t size);
uint32_t dest_ip(const byte_t* data, size_t size);
uint64_t src_mac(const byte_t* data, size_t size);
uint64_t dest_mac(const byte_t* data, size_t size);
uint16_t udp_src_port(const byte_t* data, size_t size);
uint16_t udp_dest_port(const byte_t* data, size_t size);
uint16_t tcp_src_port(const byte_t* data, size_t size);
uint16_t tcp_dest_port(const byte_t* data, size_t size);

void check_sum(byte_t* data, size_t size);

} // namespace detail
} // namespace net



#endif /* PKT_H_ */
