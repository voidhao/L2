#include <net/init.h>
#include <net/detail/thread.h>
namespace net{

bool init(){
    return true;
}

bool start(){
    detail::net_thread_t thread;
    thread.start();
    thread.stop();
    return true;
}

} // namespace net


