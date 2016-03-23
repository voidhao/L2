#ifndef _NET_NIC_IP_ADDR_H_
#define _NET_NIC_IP_ADDR_H_

namespace net{
namespace nic{

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

}} // namespace
#endif /* _NET_NIC_IP_ADDR_H_ */
