#include <strings.h>
#include <string.h>
#include <stdio.h>
#include "pkt.h"

namespace netmap{

rx_pkt::rx_pkt(dev_info* dev, byte_t* data, size_t size):
    dev_info_{dev},
    stamp_{0,0}{
    if (size <= PKT_SIZE){
        memcpy(buff_, data, size);
        size_ = size;
    } else {
        fprintf(stderr, "netmap rx_pkt too large:%ld\n", size);
        size = 0;
    }
}

rx_pkt::rx_pkt(const rx_pkt& rhs) {
    dev_info_ = rhs.dev_info_;
    stamp_ = rhs.stamp_;
    memcpy(buff_, rhs.buff_, rhs.size_);
    size_ = rhs.size_;
}

tx_pkt::tx_pkt(byte_t const* data, size_t size){
    if(size <= PKT_SIZE){
        memcpy(buff_, data, size);
        size_ = size;
    } else {
        fprintf(stderr, "netmap tx_pkt too large:%ld\n", size);
        size_ = 0;
    }
}


} // namespace





