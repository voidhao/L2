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
#include <unordered_set>
#include <algorithm>
#include <array>
#include <time.h>
#include <tins/tins.h>

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
using IP				= Tins::IP;
using UDP				= Tins::UDP;
using TCP				= Tins::TCP;
using PDU				= Tins::PDU;
using ICMP				= Tins::ICMP;
using frame_ptr			= std::shared_ptr<Tins::EthernetII>;
using IPv4Address		= Tins::IPv4Address;
using EthernetII		= Tins::EthernetII;
using VxLAN				= std::array<byte_t, 8>;
using error_t           = int;
using namespace std::placeholders;

class noncopyable{
protected:
    noncopyable(){}
    ~noncopyable(){}
private:
    noncopyable(const noncopyable&) = delete;
    noncopyable& operator=(const noncopyable&) = delete;
};

#ifndef likely
#define likely(x)	__builtin_expect(!!(x), 1)
#define unlikely(x)	__builtin_expect(!!(x), 0)
#endif /* likely and unlikely */
#define CHECK_NULL(pdu) {if(unlikely(((pdu) == nullptr))){return false;}}
#define CHECK_PDU(pdu)  if(unlikely((pdu) == nullptr)){return;}


#endif /* _COMM_DEF_H_ */
