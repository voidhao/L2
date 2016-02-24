#ifndef _NETIO_NETIO_DETAIL_H_
#define _NETIO_NETIO_DETAIL_H_

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "../netio.h"

struct nm_desc;

namespace netio{
namespace detail{
	void recieve_thread_func();
	void recieve_pkt_from_netmap(dev_info* dev);

	class dev_info_tools{
	public:
	    static dev_info* new_dev_info(nm_desc* desc);
	    static dev_info* new_dev_info(const char* dev);
	    static void delete_dev_info(dev_info* info);
	};

	class recieve_queue_t{
	public:
	    void            push(const rx_pkts& pkts);
		rx_pkts         wait();
		void stop();
	private:
		bool            run_{false};
		mutable         std::mutex mutex_;
		mutable         std::condition_variable cond_;
		rx_pkts         pkts_;
	};

	//global variables, defined in netio_detail.cpp file
    extern std::vector<dev_info_ptr>    devices;
    extern dev_info_ptr                 dev_info_reg[MAX_DEV];
    extern recieve_queue_t              recieve_queue;
    extern std::thread*                 rx_thread;
    extern std::thread*                 tx_thread;

}} // namespace

#endif /* NETIO_DETAIL_H_ */
