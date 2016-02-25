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

void statics(){
    size_t old_ipkts = netio::total_rx_pkts();
    size_t old_dpkts = netio::total_rx_pkts_size();
    for(;;){
        sleep(1);
        size_t ipkts = netio::total_rx_pkts();
        size_t dpkts = netio::total_rx_pkts_size();
        printf("%ld pps %ld bps\n", ipkts-old_ipkts, 8*(dpkts-old_dpkts));
        old_ipkts = ipkts;
        old_dpkts = dpkts;
    }
}
void process(void){
    for(;;){
        auto pkts = netio::wait_rx_pkts();
    }
}
int main(int argc, char** argv) {
	netio::open_dev("eth4");
	netio::netio_run();
	std::thread* thread1 = new std::thread(process);
	std::thread stat(statics);
	stat.join();
	thread1->join();
	delete thread1;
	netio::netio_stop();
	return 0;
}
