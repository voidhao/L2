#ifndef _NETIO_DETAIL_RECIEVER_H_
#define _NETIO_DETAIL_RECIEVER_H_

#include "detail_hdr.h"

namespace netio{
namespace detail{

class pkt_reciever_t{
public:
    size_t      rx_nr()const{return rx_nr_;}
    size_t      rx_drops()const{return rx_drops_nr_;}
    size_t      rx_size()const{return rx_size_;}
    void        start();
    void        stop();
    rx_pkts     wait();

public:
    pkt_reciever_t();
    ~pkt_reciever_t();
private:
    pkt_reciever_t(pkt_reciever_t const&) = delete;
    pkt_reciever_t& operator = (pkt_reciever_t const&) = delete;
private:
    void        push(rx_pkt_ptr pkt);
    void        push(rx_pkts& pkts, dev_info_ptr dev, size_t size);
    void        thread_func();
    void        recieve(dev_info_ptr dev);
    void        stastics_rx(dev_info_ptr dev, size_t nr, size_t size);
    void        stastics_drops(dev_info_ptr dev, size_t nr);
private:
    std::atomic_size_t          rx_nr_{0};
    std::atomic_size_t          rx_drops_nr_{0};
    std::atomic_size_t          rx_size_{0};
    std::atomic_bool            run_{false};
    mutable std::mutex          mutex_;
    mutable cond                cond_;
    rx_pkts                     pkts_;
    thread_ptr                  thread_;
};

}} // namespace

#endif /* _NETIO_DETAIL_RECIEVER_H_ */
