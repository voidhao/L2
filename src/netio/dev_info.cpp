#define NETMAP_WITH_LIBS
#include <net/netmap_user.h>
#include <atomic>
#include "hdr.h"
#include "dev_info.h"

namespace netio{


dev_info::~dev_info(){
    if(desc_){
        nm_close(desc_);
    }
}

int dev_info::fd()const{
    return desc_->fd;
}

size_t dev_info::tx_rings_nr()const{
    return desc_->nifp->ni_tx_rings;
}

size_t dev_info::rx_rings_nr()const{
    return desc_->nifp->ni_rx_rings;
}

const char* dev_info::dev_name()const{
    return desc_->nifp->ni_name;
}
void dev_info::conn_error(){
    bool expected{false};
    if(conn_error_.compare_exchange_strong(expected, true) ){
        char buff[256]{0};
        snprintf(buff, 256, "%s connection lose\n", desc_->nifp->ni_name);
        printf("%s", buff);
    }
}

void dev_info::conn_recover(){
    bool expected{true};
    if(conn_error_.compare_exchange_strong(expected, false)){
        char buff[256]{0};
        snprintf(buff, 256, "%s connection recover\n", desc_->nifp->ni_name);
        printf("%s", buff);
    }
}


} // namespace



