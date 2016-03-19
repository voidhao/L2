#include <lonlife/net/addr.h>
#include <net/detail/arp.h>

namespace net{
namespace detail{

arp_data_t arp_data;

lonlife::net::mac_addr arp_data_t::arp(uint32_t ip)const{
    uint64_t mac;
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = ip2mac_.find(ip);
    if (it != ip2mac_.end()){
        mac = it->second;
    }
    return lonlife::net::mac_addr(mac);
}
lonlife::net::mac_addr arp_data_t::arp(lonlife::net::ip4_addr ip)const{
    return arp(ip.addr());
}

} // namespace detail
} // namespace net


