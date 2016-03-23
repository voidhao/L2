#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <comm_def.h>
#include <net/detail/pkt.h>
#include <string.h>

namespace net{
namespace detail{

bool is_udp(const byte_t* data, size_t size){
    if(!ETHER_IS_VALID_LEN(size) || size <  sizeof(ether_header) + sizeof(iphdr) + sizeof(udphdr)){
        return false;
    }
    const ether_header* eth = (ether_header*)data;
    const iphdr* ip = (iphdr*)(data + sizeof(ether_header));

    return (eth->ether_type == ETHERTYPE_IP) && (ip->protocol == IPPROTO_UDP);
}
bool is_tdp(const byte_t* data, size_t size){
    if(!ETHER_IS_VALID_LEN(size) || size < sizeof(ether_header) + sizeof(iphdr) + sizeof(tcphdr)){
        return false;
    }
    const ether_header* eth = (ether_header*)data;
    const iphdr* ip = (iphdr*) (data + sizeof(ether_header));

    return (eth->ether_type == ETHERTYPE_IP) && (ip->protocol == IPPROTO_TCP);
}
bool is_arp(const byte_t* data, size_t size){
    if(!ETHER_IS_VALID_LEN(size) || size < sizeof(ether_header) +  sizeof(ether_arp)){
        return false;
    }
    const ether_header* eth = (ether_header*)data;
    return (eth->ether_type == ETHERTYPE_ARP);
}

uint32_t src_ip(const byte_t* data, size_t size){
    const iphdr* ip = (iphdr*) (data + sizeof(ether_header));
    return ntohl(ip->saddr);
}
uint32_t dest_ip(const byte_t* data, size_t size){
    const iphdr* ip = (iphdr*) (data + sizeof(ether_header));
    return ntohl(ip->daddr);
}
uint64_t src_mac(const byte_t* data, size_t size){
    uint64_t mac;
    const ethhdr* eth = (ethhdr*)data;
    memcpy(&mac, eth->h_source, ETH_ALEN);
    return mac;

}
uint64_t dest_mac(const byte_t* data, size_t size){
    uint64_t mac;
    const ethhdr* eth = (ethhdr*)data;
    memcpy(&mac, eth->h_dest, ETH_ALEN);
    return mac;
}

uint16_t udp_src_port(const byte_t* data, size_t size){
    const udphdr* udp = (udphdr*) (data + sizeof(ether_header) + sizeof(iphdr));
    return ntohs(udp->uh_sport);
}

uint16_t udp_dest_port(const byte_t* data, size_t size){
    const udphdr* udp = (udphdr*) (data + sizeof(ether_header) + sizeof(iphdr));
    return ntohs(udp->uh_dport);
}

uint16_t tcp_src_port(const byte_t* data, size_t size){
    const tcphdr* tcp = (tcphdr*) (data + sizeof(ether_header) + sizeof(tcphdr));
    return ntohs(tcp->th_sport);
}
uint16_t tcp_dest_port(const byte_t* data, size_t size){
    const tcphdr* tcp = (tcphdr*) (data + sizeof(ether_header) + sizeof(tcphdr));
    return ntohs(tcp->th_dport);
}

} // namespace detail
} // namespace net



