#ifndef _NETIO_NETIO_DETAIL_H_
#define _NETIO_NETIO_DETAIL_H_

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "../netio.h"

namespace netio{
namespace detail{
	extern std::vector<nm_desc*> all_open_devices;
	void recieve_thread_func();

	class recieve_queue{
	public:
		void wait();
		void stop()const;
	private:
		mutable bool run_{true};
		mutable std::mutex mutex_;
		mutable std::condition_variable cond_;
	};
}
}

#endif /* NETIO_DETAIL_H_ */
