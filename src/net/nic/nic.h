#ifndef _NET_NIC_H_
#define _NETNIC_H_

namespace net{
namespace nic{

const int IFNAME_SIZE = 16;

class nic{
public:
    const char*         name()const{return name_;}
    uint64_t            mac()const{return mac_;}
private:
    char                name_[IFNAME_SIZE]{0};
    uint64_t            mac_{0};
};

}} // namespace
#endif /* _NET_NIC_H_ */
