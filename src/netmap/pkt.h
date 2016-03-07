#ifndef _NETMAP_PKT_H_
#define _NETMAP_PKT_H_

#include "hdr.h"

namespace netmap{

class rx_pkt{
public:
    dev_info* dev()const{return dev_info_;}
    timeval stamp()const{return stamp_;}
    void stamp(const timeval& stamp){
        stamp_ = stamp;
    }

    rx_pkt(dev_info* dev, byte_t* data, size_t size);
    rx_pkt(const rx_pkt& rhs);
    rx_pkt copy()const{
        rx_pkt pkt = *this;
        return std::move(pkt);
    }

    size_t size()const{
        return size_;
    }

    byte_t const* data()const{
        return buff_;
    }

    ~rx_pkt(){}
private:
    rx_pkt() = delete;
    rx_pkt& operator = (const rx_pkt&) = delete;
private:
    dev_info*       dev_info_;
    timeval         stamp_;
    size_t          size_;
    byte_t          buff_[PKT_SIZE];
};

class tx_pkt{
public:
    tx_pkt(byte_t const* data, size_t size);
    byte_t const*  data()const{return buff_;}
    size_t size()const{return size_;}
private:
    tx_pkt(const tx_pkt&) = delete;
    tx_pkt& operator = (const tx_pkt&) = delete;
    size_t          size_{0};
    byte_t          buff_[PKT_SIZE];
};

}// namespace netio
#endif /* _NETMAP_PKT_H_ */
