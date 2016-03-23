#include <init.h>
#include <conf/log.h>
#include <conf/iface.h>
#include <conf/conf.h>

bool init(){
    bool ret = false;
    ret = conf::init_log();
    if(!ret){
        return ret;
    }

    conf::read_conf();
    auto ifaces = conf::read_iface();
    return true;
}


