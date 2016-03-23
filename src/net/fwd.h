#include <comm_def.h>
#ifndef _NET_FWD_H_
#define _NET_FWD_H_

namespace net{

namespace client{
class client_handler_t;
} // namespace client
class nat_handler_t;
class net_handler_t;

using client_handler_ptr = std::unique_ptr<client::client_handler_t>;
using nat_handler_ptr    = std::unique_ptr<nat_handler_t>;

extern net_handler_t net_handler;

namespace detail{

} // namespace detail
} // namespace net

#endif /* _NET_FWD_H_ */
