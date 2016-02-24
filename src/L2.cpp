//============================================================================
// Name        : L2.cpp
// Author      : vincent
// Version     :
// Copyright   : Copyright 2016 Lonlife Inc. All rights reserved
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <unistd.h>
#include "netio/netio.h"
using namespace std;

int main(int argc, char** argv) {
	netio::open_dev("eth1");
	netio::netio_run();
//    close(3);
    auto pkts = netio::wait_rx_pkts();

	netio::netio_stop();
	return 0;
}
