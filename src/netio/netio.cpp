#define NETMAP_WITH_LIBS
#include <net/netmap_user.h>
#include <stdio.h>
#include <strings.h>
#include "netio.h"
#include "detail/netio_detail.h"


namespace netio{

dev_info_ptr open_dev(const char* dev)
{
    return detail::dev_info_tools::new_dev_info(dev);
}

void close_all_dev(){
    memset(detail::dev_info_reg, 0, sizeof(detail::dev_info_reg));
    for(auto& info : detail::devices){
        detail::dev_info_tools::delete_dev_info(info);
    }
}


bool netio_run(){
    detail::pkt_reciever.start();
    detail::pkt_sender.start();
    return true;
}

void netio_stop(){
    detail::pkt_reciever.stop();
    detail::pkt_sender.stop();
    return;
}

rx_pkts wait_rx_pkts(){
    return detail::pkt_reciever.wait();
}

void send_pkt(dev_info_ptr dev, char const* data, size_t size){
    auto pkt = std::make_shared<tx_pkt>((byte_t const*)data, size);
    detail::pkt_sender.push(dev, pkt);
}

size_t  rx_nr(){
    return detail::pkt_reciever.rx_nr();
}

size_t rx_size(){
    return detail::pkt_reciever.rx_size();
}

size_t rx_drops(){
    return detail::pkt_reciever.rx_drops();
}

size_t tx_nr(){
    return detail::pkt_sender.tx_nr();
}

size_t tx_size(){
    return detail::pkt_sender.tx_size();
}

size_t tx_drops(){
    return detail::pkt_sender.tx_drops();
}

} // namespace netio











