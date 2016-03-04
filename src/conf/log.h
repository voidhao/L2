#ifndef _CONF_LOG_H_
#define _CONF_LOG_H_

#include <zlog.h>

extern zlog_category_t*    err_log;
extern zlog_category_t*    user_log;
extern zlog_category_t*    sys_log;
extern zlog_category_t*    stastics_log;

namespace conf{

    bool init_log();
    void close_log();

}; // namespace conf
#endif /* _CONF_LOG_H_ */
