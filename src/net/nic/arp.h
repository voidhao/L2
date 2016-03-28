#ifndef _NET_ARP_ARP_H_
#define _NET_ARP_ARP_H_

#include <comm_def.h>

namespace net{
namespace nic{

    class arp_cache:noncopyable{
    public:
        MACAddress arp(const IPv4Address& ip)const;
    protected:
        bool handle_arp_pkt(const EthernetII& pkt);
    private:
        using arp_map = std::unordered_map<IPv4Address, MACAddress>;
        arp_map              cache_;
        mutable std::mutex   mutex_;
    };
}

} // namespace net

#endif /* _NET_ARP_ARP_H_ */
