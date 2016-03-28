#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <error.h>
#include <sys/ioctl.h>
#include <ifaddrs.h>
#include <unistd.h>
#include <conf/SimpleIni.h>
#include <conf/log.h>

#include <net/nic/manager.h>
#include <net/nic/nic.h>

namespace net{
namespace nic{
namespace detail{

nic_manager manager;

bool nic_manager::update_nic() {
    auto ifaces = Tins::Utils::network_interfaces();
    if (ifaces.empty()) {
        zlog_error(sys_log, "System interfaces are empty");
        return false;
    }

    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sock < 0) {
        const char* msg = strerror(errno);
        zlog_error(sys_log, "Get network interfaces fail %s", msg);
        return false;
    }

    nic_vector output;
    for (auto& iface : ifaces) {
        if (iface == "lo") {
            continue;
        }
        nic_ptr nic = std::make_shared<nic_t>(iface);
        ifreq ifr;
        memset(&ifr, 0, sizeof(ifreq));
        strncpy(ifr.ifr_name, iface.c_str(), net::nic::IFNAME_SIZE);
        if (ioctl(sock, SIOCGIFHWADDR, &ifr) < 0) {
            const char* msg = strerror(errno);
            zlog_error(sys_log, "Get network interfaces fail %s", msg);
            close(sock);
            return false;
        }
        MACAddress mac_addr((byte_t*)ifr.ifr_hwaddr.sa_data);
        nic->mac(mac_addr);
        output.push_back(nic);
    }
    close(sock);
    std::unique_lock<std::mutex> lock(mutex_);
    nics_ = std::move(output);
    lock.unlock();
    dump_nic();
    return true;
}

bool nic_manager::update_nic_ip(){
    ifaddrs* ifa = nullptr;
    if(getifaddrs(&ifa) != 0){
        const char* msg = strerror(errno);
        zlog_error(sys_log, "Get ethernet ip failed: %s", msg);
        return false;
    }
    nic_ip_map output;
    ifaddrs* ifp = ifa;
    for (; ifp != nullptr; ifp = ifp->ifa_next) {
        if (ifp->ifa_addr && (ifp->ifa_addr->sa_family == AF_INET)) {
            auto nic = find_nic(ifp->ifa_name);
            if(!nic){
                continue;
            }
            sockaddr_in* addr = (sockaddr_in*) ifp->ifa_addr;
            sockaddr_in* mask = (sockaddr_in*) ifp->ifa_netmask;
            sockaddr_in* broadcast = (sockaddr_in*) ifp->ifa_broadaddr;
            nic_ip_ptr ip = std::make_shared<nic_ip>();

            ip->nic(nic);
            ip->ip(IPv4Address(addr->sin_addr.s_addr));
            ip->mask(IPv4Address(mask->sin_addr.s_addr));
            ip->broadcast(IPv4Address(broadcast->sin_addr.s_addr));
            output[addr->sin_addr.s_addr] = ip;
        }
    }
    freeifaddrs(ifa);
    unique_lock lock(mutex_);
    nic_ips_ = std::move(output);
    lock.unlock();
    dump_nic_ip();
    return true;
}
void nic_manager::dump_nic()const{
    std::unique_lock<std::mutex> lock(mutex_);
    auto nics = nics_;
    lock.unlock();
    for(auto& nic : nics){
        zlog_info(sys_log, "%s:", nic->name().c_str());
        zlog_info(sys_log, "MAC: %s", nic->mac().to_string().c_str());
    }
}

void nic_manager::dump_nic_ip()const{
    unique_lock lock(mutex_);
    auto nic_ips = nic_ips_;
    lock.unlock();
    for(auto& ip : nic_ips){
        zlog_info(sys_log, "IP:%s MASK:%s BROADCAST:%s",
                ip.second->ip().to_string().c_str(),
                ip.second->mask().to_string().c_str(),
                ip.second->broadcast().to_string().c_str());
    }
}

void nic_manager::handle_arp_pkt(const Tins::PDU& pdu){
    const ARP* arp = pdu.find_pdu<ARP>();
    if(!arp){
        return;
    }
    int opcode = arp->opcode();
    switch (opcode){
    case ARP::REQUEST:
        on_arp_request(arp);
        break;
    case ARP::REPLY:
        break;
        on_arp_reply(arp);
    default:
        break;
    }
}

void nic_manager::on_arp_request(const ARP* arp){
    auto target_ip = arp->target_ip_addr();
    auto send_hw = arp->sender_hw_addr();
    auto send_ip = arp->sender_ip_addr();
    auto nic_ip = find_nic_ip(target_ip);
    if((nic_ip)){
        if(nic_ip->nic()->nm()){
            auto resp = ARP::make_arp_reply(
                    send_ip,
                    target_ip,
                    send_hw,
                    nic_ip->nic()->mac());
            auto pkt = resp.serialize();
            lonlife::netmap::send_pkt(nic_ip->nic()->nm(),
                    (const char*)pkt.data(),
                    pkt.size());
        }
    }
    add_arp(send_ip, send_hw);
}

void nic_manager::on_arp_reply(const ARP* arp){
    auto target_ip = arp->target_ip_addr();
    auto send_hw = arp->sender_hw_addr();
    auto send_ip = arp->sender_ip_addr();
    auto nic_ip = find_nic_ip(target_ip);
    if(nic_ip){
        add_arp(send_ip, send_hw);
    }
}

} // namespace detail
} // namespace nic
} // namespace net



