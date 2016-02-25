#ifndef _NETIO_NETIO_DETAIL_H_
#define _NETIO_NETIO_DETAIL_H_

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#define NETMAP_WITH_LIBS
#include <net/netmap_user.h>
#include "../hdr.h"

namespace netio{
namespace detail{
	void recieve_thread_func();
	void send_thread_func();
	rx_pkts recieve_pkts_from_netmap(dev_info* dev);

	class dev_info_tools{
	public:
	    static dev_info* new_dev_info(nm_desc* desc);
	    static dev_info* new_dev_info(const char* dev);
	    static void delete_dev_info(dev_info* info);
	};

	class recieve_queue_t{
	public:
	    void            push(rx_pkts const& pkts);
		rx_pkts         wait();
		size_t          imm_pkt_nr()const{return imm_pkt_nr_;}
		size_t          delay_pkt_nr()const{return delay_pkt_nr_;}
		void stop();
	private:
		bool            run_{false};
		mutable         std::mutex mutex_;
		mutable         std::condition_variable cond_;
		rx_pkts         pkts_;
        std::atomic_size_t imm_pkt_nr_{0};      // 及时处理的包数
        std::atomic_size_t delay_pkt_nr_{0};    // 未及时处理的包数
	};

	class send_queue_t{
	public:
	    void            push(tx_pkt_ptr pkt);
	    tx_pkts         wait();
	private:
	    bool            run_{false};
	    mutable         std::mutex mutex_;
	    mutable         std::condition_variable cond_;
	    tx_pkts         pkts_;
	};

	//global variables, defined in netio_detail.cpp file
    extern std::vector<dev_info_ptr>    devices;
    extern dev_info_ptr                 dev_info_reg[MAX_DEV];
    extern recieve_queue_t              recieve_queue;
    extern send_queue_t                 send_queue;
    extern std::thread*                 rx_thread;
    extern std::thread*                 tx_thread;
    extern std::atomic_size_t           rx_pkts_nr;
    extern std::atomic_size_t           rx_pkts_size;
    extern std::atomic_size_t           tx_pkts_nr;
    extern std::atomic_size_t           tx_pkts_size;

}} // namespace

#endif /* NETIO_DETAIL_H_ */
