#ifndef _NET_NIC_H_
#define _NET_NIC_H_

#include <string.h>
#include <comm_def.h>
#include <lonlife/netmap.h>

namespace net{
namespace nic{

const int IFNAME_SIZE = 16;
const int MAC_LEN = 6;
const int MAC_STR_LEN = 32;

using dev_info_ptr = lonlife::netmap::dev_info_ptr;

struct nic_info{
    char                name_[IFNAME_SIZE]={0};
    byte_t              mac_[MAC_LEN]={0};
    byte_t              gw_mac_[MAC_LEN]={0};
    uint32_t            gw_ip_{0};
    char                gw_ip_str_[IFNAME_SIZE]={0};
    uint32_t            broadcast_{0};
    char                broadcast_str[IFNAME_SIZE]={0};

    dev_info_ptr        nm_dev_{nullptr};
    bool operator < (const nic_info& rhs) const{
        return strncmp(name_, rhs.name_, IFNAME_SIZE) < 0;
    }
    bool operator == (const nic_info& rhs)const{
        return strncmp(name_, rhs.name_, IFNAME_SIZE) == 0;
    }

    bool operator == (const char* name)const{
        return strncmp(name_, name, IFNAME_SIZE) == 0;
    }
};

struct ip_info{
    uint32_t            ip_{0};
    uint32_t            mask_{0};
    char                ip_str[16]{0};
    char                mask_str[16]{0};
    nic_info*           nic_{nullptr};
    dev_info_ptr        nm_dev_{nullptr};
    bool operator < (const ip_info& rhs)const{
        return (ip_ < rhs.ip_);
    }
};

class nic_manager{
public:
    enum{update_interval = 10};
    bool update_nic(const string_vector& ifaces);
    const ip_info*  find_ip(const char* ip)const;
    const ip_info*  find_ip(uint32_t ip)const;
    const nic_info* find_nic(const char* name)const{
        return find_nic(name);
    }
private:
    nic_info* find_nic(const char* name){
        auto it = std::find(nics_.begin(), nics_.end(), name);
        if (it == nics_.end()){
            return nullptr;
        }else{
            return it.base();
        }
    }

    ip_info* find_ip(const char* ip);
    ip_info* find_ip(uint32_t ip){

    }
private:
    using ips_info = std::vector<ip_info>;
    using nics_info = std::vector<nic_info>;
    bool  if_conf(const string_vector& ifaces);
    bool  nic_conf(const string_vector& ifaces);
    bool  gw_conf(const string_vector& ifaces);
private:
    time_t                  last_update_{0};
    std::mutex              mutex_;
    ips_info                ips_;
    nics_info               nics_;
};

} // namespace nic
} // namespace
#endif /* _NET_NIC_H_ */
