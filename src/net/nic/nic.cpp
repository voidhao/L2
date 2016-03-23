#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <error.h>
#include <sys/ioctl.h>
#include <netinet/ether.h>
#include <ifaddrs.h>
#include <unistd.h>

#include <conf/SimpleIni.h>
#include <net/nic/nic.h>
#include <conf/log.h>


namespace net{
namespace nic{


bool nic_manager::update_nic(const string_vector& ifaces){
    time_t now = time(NULL);
    if((now - last_update_) < update_interval){
        return true;
    }
    last_update_ = now;
    nic_conf(ifaces);
    gw_conf(ifaces);
    if_conf(ifaces);
    return true;
}

bool nic_manager::nic_conf(const string_vector& ifaces){
    ifreq ifr;
    nics_info nics;
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if(sock < 0){
        const char* msg = strerror(errno);
        zlog_error(sys_log, "Get network interfaces fail %s", msg);
        return false;
    }

    for(auto& iface : ifaces){
        nic_info nic;
        memset(&ifr, 0, sizeof(ifreq));
        strncpy(nic.name_, iface.c_str(), IFNAME_SIZE);
        strncpy(ifr.ifr_name, iface.c_str(), IFNAME_SIZE);
        if(ioctl(sock, SIOCGIFHWADDR, &ifr) < 0){
            const char* msg = strerror(errno);
            zlog_error(sys_log, "Get network interfaces fail %s", msg);
            close(sock);
            return false;
        }
        memcpy(nic.mac_, ifr.ifr_hwaddr.sa_data, MAC_LEN);

        memset(&ifr, 0, sizeof(ifreq));
        strncpy(ifr.ifr_name, iface.c_str(), IFNAME_SIZE);
        if(ioctl(sock, SIOCGIFBRDADDR, &ifr) < 0){
            const char* msg = strerror(errno);
            zlog_error(sys_log, "Get network gateway fail %s", msg);
            close(sock);
            return false;
        }
        sockaddr_in* brd = (sockaddr_in*)&ifr.ifr_addr;
        nic.broadcast_ = ntohl(brd->sin_addr.s_addr);
        const char* brd_str = inet_ntoa(brd->sin_addr);
        strncpy(nic.broadcast_str, brd_str, IFNAME_SIZE);

        nics.push_back(nic);
    }
    close(sock);
    std::sort(nics.begin(), nics.end());
    std::lock_guard<std::mutex> lock(mutex_);
    nics_ = std::move(nics);
    return true;
}

bool nic_manager::if_conf(const string_vector& ifaces){
    ifaddrs* ifa = nullptr;
    if(getifaddrs(&ifa) != 0){
        const char* msg = strerror(errno);
        zlog_error(sys_log, "Get network interfaces fail %s", msg);
        return false;
    }
    ips_info ips;
    ifaddrs* ifp = ifa;
    for(; ifp != nullptr; ifp = ifp->ifa_next){
        if(ifp->ifa_addr && (ifp->ifa_addr->sa_family == AF_INET)){
            const char* ifname = ifp->ifa_name;
            auto it = std::find(ifaces.begin(), ifaces.end(), ifname);
            if(it == ifaces.end()){
                continue;
            }

            ip_info ip;
            sockaddr_in* addr = (sockaddr_in*) ifp->ifa_addr;
            sockaddr_in* mask = (sockaddr_in*) ifp->ifa_netmask;

            uint32_t ip_addr = ntohl(addr->sin_addr.s_addr);
            const char* ip_str = inet_ntoa(addr->sin_addr);
            strncpy(ip.ip_str, ip_str, 16);
            uint32_t mask_ip = ntohl(mask->sin_addr.s_addr);
            const char* mask_str =  inet_ntoa(mask->sin_addr);
            strncpy(ip.mask_str, mask_str, 16);

            ip.ip_ = ip_addr;
            ip.mask_ = mask_ip;
            ips.push_back(ip);
        }
    }
    freeifaddrs(ifa);
    std::sort(ips.begin(), ips.end());
    std::lock_guard<std::mutex> lock(mutex_);
    ips_ = std::move(ips);
    return true;
}

bool nic_manager::gw_conf(const string_vector& ifaces){
    static const char* gateway_conf_file = "conf/gateway.ini";
    CSimpleIniA ini;
    int err = ini.LoadFile(gateway_conf_file);
    if (err != 0){
        zlog_error(sys_log, "Gateway configure file load failed");
        return false;
    }
    for(auto& iface : ifaces){
        auto nic = find_nic(iface.c_str());
        if(nic == nullptr){
            continue;
        }
        const char* ip_str = ini.GetValue(iface.c_str(), "ip", "");
        const char* mac_str = ini.GetValue(iface.c_str(), "mac", "");
        if(strnlen(ip_str, IFNAME_SIZE) == 0 || strnlen(mac_str, 32) == 0){
            zlog_error(sys_log, "Gateway %s configure error", iface.c_str());
            return false;
        }
        in_addr sin;
        err = inet_aton(ip_str,(in_addr*)&sin);
        if(err == 0){
            zlog_error(sys_log, "%s %s invalid", gateway_conf_file, ip_str);
            return false;
        }
        strncpy(nic->gw_ip_str_, ip_str, IFNAME_SIZE);
        nic->gw_ip_ = ntohl(sin.s_addr);
        auto eth = ether_aton(mac_str);
        memcpy(nic->gw_mac_, eth->ether_addr_octet, MAC_LEN);
    }
    return true;
}


} // namespace nic
} // namespace net



