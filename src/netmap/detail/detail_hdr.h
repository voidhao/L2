#ifndef _NETMAP_DETAIL_DETAIL_HDR_H_
#define _NETMAP_DETAIL_DETAIL_HDR_H_

#include <mutex>
#include <condition_variable>
#include <atomic>
#include <utility>
#include <thread>
#define NETMAP_WITH_LIBS
#include <net/netmap_user.h>
#include "../hdr.h"

namespace netmap{
namespace detail{

using cond = std::condition_variable;
using thread_ptr = std::unique_ptr<std::thread>;

const int  TX_BUFF_SIZE =  8192;
const int  RX_BUFF_SIZE =  1024 * 10;

}}

#endif /* _NETMAP_DETAIL_DETAIL_HDR_H_ */
