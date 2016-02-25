#ifndef _NETIO_NETIO_H_
#define _NETIO_NETIO_H_

#include <sys/time.h>
#include <atomic>
#include "hdr.h"
#include "pkt.h"

namespace netio{

struct pkt {
    size_t          dev_{0};
    size_t          size_{0};
    byte_t          data_[MTU_SIZE]{0};
    timeval         stamp_{0,0};
};

namespace detail{
    class dev_info_tools;
}

class dev_info{
public:
    const char*     dev_name()const;
    nm_desc*        desc()const{return desc_;}
    size_t          tx_pkt_count()const{return tx_pkt_count_;}
    size_t          rx_pkt_count()const{return rx_pkt_count_;};
    size_t          tx_rings_nr()const;
    size_t          rx_rings_nr()const;
    int             fd()const;
protected:
    dev_info(nm_desc* desc):desc_(desc){};
    ~dev_info();
    friend detail::dev_info_tools;
protected:
    std::atomic_uint tx_pkt_count_{0};
    std::atomic_uint rx_pkt_count_{0};
    std::atomic_size_t tx_size_{0};
    std::atomic_size_t rx_size_{0};
private:
    nm_desc*         desc_{nullptr};
};
using dev_info_ptr = dev_info*;


dev_info_ptr  open_dev(const char*);
void          close_all_dev();
bool          netio_run();
void          netio_stop();
rx_pkts       wait_rx_pkts();
void          dump_dev();
size_t        imm_pkts_nr();
size_t        delay_pkts_nr();
size_t        total_rx_pkts();
size_t        total_rx_pkts_size();

} // namespace netio
#endif /* _NETIO_NETIO_H_ */
