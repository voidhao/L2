#ifndef _NET_NET_HANDLER_H_
#define _NET_NET_HANDLER_H_

#include <net/fwd.h>
#include <comm_def.h>
#include <lonlife/netmap.h>
#include <net/client/client_handler.h>
namespace net {

class net_handler_t : noncopyable{
public:
    net_handler_t();
    ~net_handler_t();
    void start();
    void stop();
private:
    void thread_func();
    bool pkts_handler(lonlife::netmap::rx_pkt_ptr& pkt);
    bool arp_handler(lonlife::netmap::rx_pkt_ptr& pkt);
    bool client_handler(lonlife::netmap::rx_pkt_ptr& pkt);
    std::atomic_bool    run_{false};
private:
    string_vector       nics_;
    thread_ptr          thread_;
    client_handler_ptr  client_handler_{new client::client_handler_t};
};

extern net_handler_t net_handler;

} /* namespace net */

#endif /* _NET_NET_HANDLER_H_ */
