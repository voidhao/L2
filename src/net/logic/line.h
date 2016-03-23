#ifndef _NET_LOGIC_LINE_H_
#define _NET_LOGIC_LINE_H_

struct line_node_t{
    node_id         id_;
    uint32_t        ip_;
    uint64_t        port_;
};

struct line_t{
    line_id         id_;
    std::vector<line_node_t> nodes_;
};


#endif /* _NET_LOGIC_LINE_H_ */
