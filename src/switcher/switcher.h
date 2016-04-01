#ifndef _LONLIFE_SWITCH_SWITCHER_H_
#define _LONLIFE_SWITCH_SWITCHER_H_

#include <comm_def.h>

namespace client{
class client_manager;
}

namespace switcher{
using client_mgr_ptr = std::unique_ptr<client::client_manager>;

class switcher{
public:
private:
	client_mgr_ptr	client_mgr_;
};

}

#endif /* _LONLIFE_SWITCH_SWITCH_H_ */
