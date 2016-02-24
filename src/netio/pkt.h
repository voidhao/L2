#ifndef _NETIO_PKT_H_
#define _NETIO_PKT_H_

#include "hdr.h"

namespace netio{

struct pkt_buff{
    uint32_t        size_;
    byte_t          data_[MTU_SIZE];
    pkt_buff():size_{0}{
    }
    pkt_buff(byte_t* data, size_t size){
        if(size <= MTU_SIZE){
            memcpy(data_, data, size);
            size_ = size;
        } else {
            size_ = 0;
        }
    }
};

class rx_pkt{
public:
    const dev_info* dev()const{return dev_info_;}
    timeval stamp()const{return stamp_;}
    void stamp(const timeval& stamp){
        stamp_ = stamp;
    }

    rx_pkt(dev_info* dev, byte_t* data, size_t size):
        dev_info_{dev},
        stamp_{0,0}{
        if (size <= MTU_SIZE){
            memcpy(buff_, data, MTU_SIZE);
            size_ = size;
        } else {
            size = 0;
        }
    }

    rx_pkt(const rx_pkt& rhs) {
        dev_info_ = rhs.dev_info_;
        stamp_ = rhs.stamp_;
        memcpy(buff_, rhs.buff_, rhs.size_);
        size_ = rhs.size_;
    }

    rx_pkt copy()const{
        rx_pkt pkt = *this;
        return std::move(pkt);
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

}// namespace netio
#endif /* _NETIO_PKT_H_ */
