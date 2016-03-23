#include <lonlife/net/addr.h>
#include <net/detail/arp.h>

namespace net{

lonlife::net::mac_addr arp(uint32_t ip){
    return detail::arp_data.arp(ip);
}

} // namespace net

