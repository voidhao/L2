#ifndef _NET_DETAIL_ARP_H_
#define _NET_DETAIL_ARP_H_

#include <unordered_map>
#include <mutex>

namespace net{
namespace detail{

class arp_data_t{
public:
    lonlife::net::mac_addr arp(uint32_t ip)const;
    lonlife::net::mac_addr arp(lonlife::net::ip4_addr ip)const;
protected:
    void add_ip2mac(uint32_t ip, uint64_t mac);
//    friend void net::handle_arp(const char*, size_t);
private:
    std::unordered_map<uint32_t, uint64_t> ip2mac_;
    std::unordered_multimap<uint64_t, uint32_t> mac2ip_;
    mutable std::mutex mutex_;
};

extern arp_data_t arp_data;

} // namespace detail
} // namespace net



#endif /* _NET_DETAIL_ARP_H_ */
