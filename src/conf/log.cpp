#include "log.h"

zlog_category_t*    sys_log = nullptr;
zlog_category_t*    user_log;
zlog_category_t*    stastics_log;
zlog_category_t*    err_log;

namespace conf{

void close_log(){
    zlog_fini();
}

bool init_log(){
    int rc;
    rc = zlog_init("conf/log.conf");
    if(rc){
        printf("init log failed\n");
        return false;
    }

    sys_log = zlog_get_category("sys_log");
    if (!sys_log) {
    printf("system log fail\n");
        zlog_fini();
        return false;
    }

    user_log = zlog_get_category("user_log");
    if (!user_log) {
    printf("user log fail\n");
        zlog_fini();
        return false;
    }

    stastics_log = zlog_get_category("stastics_log");
    if (!stastics_log) {
    printf("stastics log fail\n");
        zlog_fini();
        return false;
    }

    err_log = zlog_get_category("err_log");
    if (!err_log) {
    printf("error log fail\n");
        zlog_fini();
        return false;
    }

    return true;
}

}



