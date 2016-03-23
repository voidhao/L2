#ifndef _NET_CLIENT_USER_H_
#define _NET_CLIENT_USER_H_

namespace net{
namespace client{

using session_id = uint64_t;

session_id make_session_id(uint32_t ip, uint16_t port){
     return ((uint64_t (ip)) << 32) | (port);
}

enum class user_stat{

};

struct session:noncopyable{
    uint32_t    origin_vxlanid_{0};
    uint32_t    inner_vxlanid_{0};
    uint32_t    wan_ip_{0};
    uint16_t    wan_port_{0};
    uint32_t    lan_ip_{0};
    uint16_t    lan_port_{0};
    uint64_t    mac_{0};
    uint64_t    rx_size_{0};
    uint64_t    tx_size_{0};
};


struct user_t:noncopyable{
public:
    uint64_t        uid_{0};
    int             user_level_{0};
    uint64_t        line_id_{0};
    user_stat       stat_;
    time_t          init_stamp_{0};
    time_t          conn_stamp_{0};
    time_t          disconn_stamp_{0};
    session         session_;
};

} // namespace client
} // namespace net



#endif /* _NET_CLIENT_USER_H_ */
