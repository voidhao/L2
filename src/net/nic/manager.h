#ifndef _NET_NIC_MANAGER_H_
#define _NET_NIC_MANAGER_H_

#include <comm_def.h>
#include <net/nic/ip_addr.h>
#include <net/nic/nic.h>

namespace net{
namespace nic{
namespace detail{

class nic_manager{
public:
    enum{update_interval = 10};
    const_nic_ip_ptr  find_ip(uint32_t ip)const{
        uint32_t addr = IPv4Address(ip);
        return find_ip_ptr(addr);
    }
    const_nic_ptr   find_nic(const std::string& name)const{
        return find_nic_ptr(name.c_str());
    }
    const_nic_ptr   find_nic(const char* name)const{
        return find_nic_ptr(name);
    }
    MACAddress arp(uint32_t ip)const{
        lock_guard lock(mutex_);
        auto it = arps_.find(IPv4Address(ip));
        if(it != arps_.end()){
            return it->second;
        }else{
            return MACAddress();
        }
    }

    bool update_nic();
    bool update_nic_ip();
    void dump_nic()const;
    void dump_nic_ip()const;
    void on_nic_opened(const char* name, lonlife::netmap::dev_info_ptr dev){
        auto nic = find_nic_ptr(name);
        if(nic){
            nic->nm(dev);
        }else{
            printf("%s not found\n", name);
        }
    }
    void handle_arp_pkt(const Tins::PDU& pdu);
private:
    // 获取网关和路由信息比较麻烦，暂时用配置文件代替
    bool update_gateway_form_config_file();
    nic_ptr find_nic_ptr(const char* name)const{
        unique_lock lock(mutex_);
       for(auto& nic : nics_){
           if(nic->name() == name){
               return nic;
           }
       }
       return nullptr;
    }

    nic_ip_ptr find_ip_ptr(uint32_t ip)const{
        lock_guard lock(mutex_);
        auto it = nic_ips_.find(ip);
        if(it != nic_ips_.end()){
            return it->second;
        } else {
            return nullptr;
        }
    }
    void on_arp_request(const ARP* arp);
    void on_arp_reply(const ARP* arp);
    void add_arp(IPv4Address ip, MACAddress mac){
        lock_guard lock(mutex_);
        arps_[ip] = mac;
    }
private:
private:
    using nic_vector        = std::vector<nic_ptr>;
    using nic_ip_map        = std::unordered_map<uint32_t, nic_ip_ptr>;
    using arp_map           = std::unordered_map<IPv4Address, MACAddress>;
    time_t                  last_update_{0};
    mutable std::mutex      mutex_;
    nic_vector              nics_;
    nic_ip_map              nic_ips_;
    arp_map                 arps_;
};

extern nic_manager manager;

} // namespace detail
} // namespace nic
} // namespace net



#endif /* _NET_NIC_MANAGER_H_ */
