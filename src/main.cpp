

#include <unistd.h>
#include <net/nic/nic.h>
#include <lonlife/netmap.h>
#include "init.h"

int main(int argc, char** argv) {
    init();
    net::nic::init();

    auto dev = lonlife::netmap::open_dev("eth0");
    net::nic::nic_opened("eth0", dev);
    lonlife::netmap::netio_run();
    for(;;){
        auto pkts = lonlife::netmap::wait_rx_pkts();
        for(auto& pkt : pkts){
            auto pdu = Tins::EthernetII(pkt->data(), pkt->size());
            net::nic::handle_arp_pkt(pdu);
        }
    }
    return 0;
}
