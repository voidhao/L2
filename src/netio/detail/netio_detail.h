#ifndef _NETIO_NETIO_DETAIL_H_
#define _NETIO_NETIO_DETAIL_H_

#include "detail_hdr.h"
#include "reciever.h"
#include "send.h"

namespace netio{
namespace detail{


void recieve_thread_func();
void send_thread_func();


class dev_info_tools{
public:
    static dev_info* new_dev_info(nm_desc* desc);
    static dev_info* new_dev_info(const char* dev);
    static void delete_dev_info(dev_info* info);
};

//global variables, defined in netio_detail.cpp file
extern std::vector<dev_info_ptr>    devices;
extern dev_info_ptr                 dev_info_reg[MAX_DEV];
extern pkt_sender_t                 pkt_sender;
extern pkt_reciever_t               pkt_reciever;


}} // namespace
#endif /* NETIO_DETAIL_H_ */
