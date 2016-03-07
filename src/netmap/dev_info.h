#ifndef _NETMAP_DEV_INFO_H_
#define _NETMAP_DEV_INFO_H_

namespace netmap{

class dev_info{
public:
    const char*     dev_name()const;
    nm_desc*        desc()const{return desc_;}
    size_t          tx_rings_nr()const;
    size_t          rx_rings_nr()const;
    int             fd()const;
protected:
    dev_info(nm_desc* desc):desc_(desc){};
    ~dev_info();
    void conn_error();
    void conn_recover();
    friend detail::dev_info_tools;
    friend detail::pkt_sender_t;
    friend detail::pkt_reciever_t;
protected:
    std::atomic_size_t rx_nr_{0};
    std::atomic_size_t rx_drops_nr_{0};
    std::atomic_size_t rx_size_{0};
    std::atomic_size_t tx_size_{0};
    std::atomic_size_t tx_nr_{0};
    std::atomic_size_t tx_drops_nr_{0};
    std::atomic_bool   conn_error_{false};
    tx_pkts            tx_pkts_buff_;
private:
    nm_desc*         desc_{nullptr};
};


} // namespace
#endif /* _NETMAP_DEV_INFO_H_ */
