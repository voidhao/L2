#include <net/init.h>
#include <net/net_handler.h>
namespace net{

bool init(){
    net_handler.stop();
    return true;
}

bool start(){
    net_handler.start();
    return true;
}

} // namespace net


