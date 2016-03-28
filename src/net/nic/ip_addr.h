#ifndef _NET_NIC_IP_ADDR_H_
#define _NET_NIC_IP_ADDR_H_

#include <lonlife/netmap.h>
#include <comm_def.h>

namespace net{
namespace nic{

namespace detail{
    class nic_manager;
}

using ip_addr_t =   uint32_t;

class ip_addr{
public:
    ip_addr_t           ip()const{return ip_;}
    ip_addr_t           mask()const{return mask_;}
    ip_addr_t           gateway()const{return gateway_;}
public:
    bool operator == (const ip_addr& rhs)const{
        return (ip_ == rhs.ip_) &&
                (mask_ == rhs.mask_) &&
                (gateway_ == rhs.gateway_);
    }

    ip_addr(const ip_addr& rhs):
        ip_(rhs.ip_),
        mask_(rhs.mask_),
        gateway_(rhs.gateway_){
    }

    ip_addr& operator = (ip_addr const& rhs){
        ip_ = rhs.ip_;
        mask_ = rhs.mask_;
        gateway_ = rhs.gateway_;
        return *this;
    }

    ip_addr() = default;
    ~ip_addr() = default;

private:
    ip_addr_t           ip_{0};
    ip_addr_t           mask_{0};
    ip_addr_t           gateway_{0};
};

class nic_t:noncopyable{
public:
    std::string         name()const{
        return name_;
    }
    MACAddress          mac()const{
        return mac_;
    }
    IPv4Address         gateway_ip()const{
        return gw_ip_;
    }
    MACAddress          gateway_mac()const{
        return gw_mac_;
    }

    bool operator == (const nic_t& rhs){
        return name_ == rhs.name_;
    }

    bool operator < (const nic_t& rhs){
        return name_ < rhs.name_;
    }

    lonlife::netmap::dev_info_ptr nm()const{
        return nm_;
    }


    nic_t(const std::string& name):name_(name), nm_(nullptr){};
    ~nic_t(){};
protected:
    friend detail::nic_manager;
    void name(const std::string& name){
        name_ = name;
    }
    void mac(const MACAddress& mac){
        mac_ = mac;
    }
    void gateway_ip(const IPv4Address& ip){
        gw_ip_ = ip;
    }
    void gateway_mac(const MACAddress& mac){
        gw_mac_ = mac;
    }
    void nm(lonlife::netmap::dev_info_ptr nm){
        nm_ = nm;
    }
private:
    std::string         name_;
    MACAddress          mac_;
    MACAddress          gw_mac_;
    IPv4Address         gw_ip_;
    lonlife::netmap::dev_info_ptr nm_;
};

using nic_ptr = std::shared_ptr<nic_t>;
using const_nic_ptr = std::shared_ptr<const nic_t>;

class nic_ip:noncopyable{
public:
    nic_ip(){}
    ~nic_ip(){}
    IPv4Address     ip()const{
        return ip_;
    }
    IPv4Address     mask()const{
        return mask_;
    }
    IPv4Address     broadcast()const{
        return broadcast_;
    }
    const_nic_ptr   nic()const{
        return nic_;
    }
protected:
    friend detail::nic_manager;
    void ip(const IPv4Address& ip){
        ip_ = ip;
    }
    void mask(const IPv4Address& ip){
        mask_ = ip;
    }
    void broadcast(const IPv4Address& ip){
        broadcast_  = ip;
    }
    void nic(const_nic_ptr nic){
        nic_ = nic;
    }
private:
    IPv4Address         ip_;
    IPv4Address         mask_;
    IPv4Address         broadcast_;
    const_nic_ptr       nic_;
};

using nic_ip_ptr  = std::shared_ptr<nic_ip>;
using const_nic_ip_ptr = std::shared_ptr<const nic_ip>;

} // namespace nic
} // namespace net
#endif /* _NET_NIC_IP_ADDR_H_ */
