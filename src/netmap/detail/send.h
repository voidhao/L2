#ifndef _NETMAP_DETAIL_SEND_H_
#define _NETMAP_DETAIL_SEND_H_


#include "detail_hdr.h"

namespace netmap{
namespace detail{

using dev_tx_queue      = std::pair<dev_info_ptr, tx_pkts>;
using dev_tx_queues     = std::vector<dev_tx_queue>;

class pkt_sender_t{
public:
    bool push(dev_info_ptr dev, tx_pkt_ptr pkt);
    void start();
    void stop();
    size_t                  tx_nr() const{return tx_nr_;}
    size_t                  tx_size()const{return tx_size_;}
    size_t                  tx_drops()const{return tx_drops_nr_;}
public:
    pkt_sender_t();
    ~pkt_sender_t();
private:
    pkt_sender_t(const pkt_sender_t&) = delete;
    pkt_sender_t& operator=(pkt_sender_t const&) = delete;
private:
    dev_tx_queues wait();
    void send_pkts_to_netmap(dev_tx_queue const&);
    void thread_func();
    void stastics_tx(dev_info_ptr dev, size_t nr, size_t size);
    void stastics_drops(dev_info_ptr dev, size_t nr);
private:
    std::atomic_bool       run_{false};
    std::atomic_size_t     tx_nr_{0};
    std::atomic_size_t     tx_drops_nr_{0};
    std::atomic_size_t     tx_size_{0};
    mutable std::mutex     mutex_;
    mutable cond           cond_;
    thread_ptr             thread_;
};

}} // namesapce
#endif /* _NETMAP_DETAIL_SEND_H_ */
