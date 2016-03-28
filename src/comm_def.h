#ifndef _COMM_DEF_H_
#define _COMM_DEF_H_

#include <stdint.h>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <utility>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <unordered_map>
#include <algorithm>
#include <time.h>
#include <tins/tins.h>

using namespace Tins;

using byte_t            = unsigned char;
using string_vector     = std::vector<std::string>;
using cond              = std::condition_variable;
using thread_ptr        = std::unique_ptr<std::thread>;
using line_id           = uint64_t;
using node_id           = uint64_t;
using mac_address       = Tins::HWAddress<6>;
using MACAddress        = Tins::HWAddress<6>;
using unique_lock       = std::unique_lock<std::mutex>;
using lock_guard        = std::lock_guard<std::mutex>;

class noncopyable{
protected:
    noncopyable(){}
    ~noncopyable(){}
private:
    noncopyable(const noncopyable&) = delete;
    noncopyable& operator=(const noncopyable&) = delete;
};


#endif /* _COMM_DEF_H_ */
