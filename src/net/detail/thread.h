#ifndef _NET_DETAIL_THREAD_H_
#define _NET_DETAIL_THREAD_H_

#include <memory>
#include <thread>
#include <atomic>

namespace net{
namespace detail{

class net_thread_t{
public:
    void start();
    void stop();
public:
    net_thread_t();
    ~net_thread_t();
private:
    void thread_func();
    net_thread_t(net_thread_t const&) = delete;
    net_thread_t& operator = (net_thread_t const&) = delete;
private:
    std::atomic_bool  run_{false};
    std::unique_ptr<std::thread> thread_;
};


} // namespace detail
} // namespace net
#endif /* _NET_DETAIL_THREAD_H_ */
