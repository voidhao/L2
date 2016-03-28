#include <net/nic/arp.h>

namespace net{
namespace nic{

MACAddress arp_cache::arp(const IPv4Address& ip)const{
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = cache_.find(ip);
    if(it != cache_.end()){
        return it->second;
    } else {
        return MACAddress();
    }
}

bool arp_cache::handle_arp_pkt(const EthernetII& pkt){
    auto arp = pkt.find_pdu<ARP>();
    if(!arp){
        return true;
    }

    auto opcode = arp->opcode();
    return true;
}

} // namespace nic
} // namespace net

