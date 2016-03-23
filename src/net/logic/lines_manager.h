#ifndef LINES_MANAGER_H_
#define LINES_MANAGER_H_

#include <comm_def.h>

namespace net{
namespace logic{

class node_t;
class line_t;

class lines_manager:noncopyable{
public:
    lines_manager(){};
    ~lines_manager(){};
private:
    using node_ptr = std::shared_ptr<node_t>;
    using line_ptr = std::shared_ptr<line_t>;
    std::mutex       mutex_;
    std::vector<node_ptr> nodes_;
    std::vector<line_ptr> lines_;
};

} // namesapce logic
} // namespace net


#endif /* LINES_MANAGER_H_ */
