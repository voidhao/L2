//============================================================================
// Name        : L2.cpp
// Author      : vincent
// Version     :
// Copyright   : Copyright 2016 Lonlife Inc. All rights reserved
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <unistd.h>
#include <thread>
#include "netio/netio.h"
using namespace std;

size_t total_pkts = 0;
size_t total_size = 0;
time_t stamp;

netio::dev_info_ptr eth1;

void statics(){
    sleep(2);
    for(;;){
        char msg[] = "hello, world, hello world, hello world, hello world, hello world";
        netio::send_pkt(eth1, msg, 50);
    }
}

void stastics(){
    size_t rx_nr, rx_size, rx_drops;
    size_t tx_nr, tx_size, tx_drops;
    rx_nr = netio::rx_nr();
    rx_size = netio::rx_size();
    rx_drops = netio::rx_drops();
    tx_nr = netio::tx_nr();
    tx_size = netio::tx_size();
    tx_drops = netio::tx_drops();
    int interval = 5;
    for(;;){
        sleep(interval);
        size_t n_rx_nr = netio::rx_nr();
        size_t n_rx_size = netio::rx_size();
        size_t n_rx_drops = netio::rx_drops();
        size_t n_tx_nr = netio::tx_nr();
        size_t n_tx_size = netio::tx_size();
        size_t n_tx_drops = netio::tx_drops();
        size_t d_rx_nr = (n_rx_nr - rx_nr)/interval;
        size_t d_rx_size = (n_rx_size - rx_size)/interval;
        size_t d_rx_drops = (n_rx_drops -rx_drops)/interval;
        size_t d_tx_nr = (n_tx_nr - tx_nr)/interval;
        size_t d_tx_size = (n_tx_size - tx_size)/interval;
        size_t d_tx_drops = (n_tx_drops - tx_drops)/interval;
        rx_nr = n_rx_nr;
        rx_size = n_rx_size;
        rx_drops = n_rx_drops;
        tx_nr = n_tx_nr;
        tx_size = n_tx_size;
        tx_drops = n_tx_drops;
        printf("Total:\n");
        printf("rx nr:%10ld size:%10ld drops:%10ld\n", rx_nr, rx_size, rx_drops);
        printf("rx nr:%10ld size:%10ld drops:%10ld\n", d_rx_nr, d_rx_size, d_rx_drops);
        printf("tx nr:%10ld size:%10ld drops:%10ld\n", d_tx_nr, d_tx_size, d_tx_drops);
        printf("---------------------------------\n");
    }
}
void process(void){
    for(;;){
        auto pkts = netio::wait_rx_pkts();
    }
}
int main(int argc, char** argv) {
	eth1 = netio::open_dev("eth2");
	netio::netio_run();
	std::thread* thread1 = new std::thread(process);
    std::thread* thread2 = new std::thread(statics);
    std::thread* thread3 = new std::thread(stastics);
    for(;;){
        netio::wait_rx_pkts();
    }
	netio::netio_stop();
	return 0;
}
