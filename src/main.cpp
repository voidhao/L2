

#include <unistd.h>
#include <comm_def.h>
#include <netmap/netmap.hpp>
#include "init.h"
#include <client/client.h>

using namespace lonlife;

int main(int argc, char** argv) {
    init();
    netmap::init();
    netmap::open_dev("eth1", IPv4Address("10.0.0.1"));
    netmap::listen(IPv4Address("0.0.0.0"),
    		IPv4Address("0.0.0.0"),
    		IPPROTO_UDP, 4789,
    		std::bind(client::client_pkt_handler, _1));
    netmap::start();
    sleep(10000);
}
