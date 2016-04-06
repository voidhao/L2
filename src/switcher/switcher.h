#ifndef _LONLIFE_SWITCH_SWITCHER_H_
#define _LONLIFE_SWITCH_SWITCHER_H_

#include <comm_def.h>

namespace client{
class client_manager;
}

namespace nat{
class nat;
}
namespace switcher{

class path_manager;

using client_mgr_ptr = std::unique_ptr<client::client_manager>;
using path_mgr_ptr = std::unique_ptr<path_manager>;
using nat_ptr = std::unique_ptr<nat::nat>;

class switcher:noncopyable{
public:
    // 数据包入口,这些数据都已经进行过IP分片重组

    // 来自客户端数据入口.
    // 检查Outer, Inner数据, 然后调用路径判断逻辑
    bool on_client_pkt(const byte_t* data, size_t size);

    // 来自其他节点的数据入口
    // 检查Outer, Inner数据, 然后调用路径判断逻辑
    void on_switcher_pkt(const byte_t* data, size_t size);

    // 其他数据包判断是否NAT数据,如果不是则直接丢弃
    // 合法数据送到NAT逻辑
    void on_nat_pkt(const byte_t* data, size_t size);

    bool init();
    bool start();
    void stop();
public:
	switcher();
    ~switcher();
private:
	client_mgr_ptr	client_mgr_;
	path_mgr_ptr	path_mgr_;
	nat_ptr			nat_;
};

}

#endif /* _LONLIFE_SWITCH_SWITCH_H_ */
