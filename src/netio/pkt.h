#ifndef _NETIO_PKT_H_
#define _NETIO_PKT_H_

#include "hdr.h"

namespace netio{

class rx_pkt{
public:
    const dev_info* dev()const{return dev_info_;}
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
    byte_t          buff_[MTU_SIZE];
};

class tx_pkt{
public:
    tx_pkt(dev_info* dev, byte_t const* data, size_t size);
    dev_info const* dev()const{return dev_info_;}
    byte_t const*  data()const{return buff_;}
private:
    tx_pkt(const tx_pkt&) = delete;
    tx_pkt& operator = (const tx_pkt&) = delete;
    dev_info*       dev_info_{nullptr};
    size_t          size_{0};
    byte_t          buff_[MTU_SIZE];
};

}// namespace netio
#endif /* _NETIO_PKT_H_ */
