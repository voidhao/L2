#include "netio_detail.h"

namespace netio{
namespace detail{

std::vector<nm_desc*> all_open_devices;


void recieve_thread_func(){
}


void recieve_queue::stop()const{
	std::lock_guard<std::mutex> lock(mutex_);
	run_ = false;
}

void recieve_queue::wait(){

}

}} // namespace
