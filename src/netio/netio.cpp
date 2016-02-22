#define NETMAP_WITH_LIBS
#include <net/netmap_user.h>
#include <stdio.h>
#include <strings.h>
#include "netio.h"
#include "detail/netio_detail.h"


using namespace netio;

int netio::open_dev(const char* dev)
{
	// check double open
	for(auto& desc :detail::all_open_devices){
		int n = strcmp(desc->nifp->ni_name, dev);
		if (n == 0){
			return -1;
		}
	}

	char buff[256]={0};
	snprintf(buff, 256, "netmap:%s", dev);
	nm_desc* desc = nm_open(buff, nullptr, 0, 0);
	if(desc){
		detail::all_open_devices.push_back(desc);
		return detail::all_open_devices.size();
	} else {
		return -1;
	}
}

void netio::close_all_dev(){
	for(auto& desc : detail::all_open_devices){
		nm_close(desc);
	}
}

void netio::dump_dev(){
	for(auto& desc : detail::all_open_devices){
		printf("%s\n", desc->nifp->ni_name);
	}
}

void netio::wait_pkt(pkt_handler handler){

}















