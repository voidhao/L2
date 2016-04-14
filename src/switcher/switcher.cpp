#include "switcher.h"
#include "tuple.h"
#include "directions.h"
#include <client/client_manager.h>
#include <client/session.h>
#include "path_manager.h"
#include <nat/nat.h>
#include <common/errors.h>

namespace switcher {
    switcher::switcher() :
            client_mgr_{new client::client_manager},
            path_mgr_{new path_manager},
            nat_{new nat::nat}{
    }

    switcher::~switcher() {

    }

    bool switcher::on_client_pkt(const byte_t *data, size_t size) {
        vxlan_tuple tuple;
        int e = tuple.fill(data, size);
        if (e != 0) {
            return true;
        }
        auto sess = client_mgr_->client_tuple(tuple);
        if(unlikely(!sess)){
        	return false;
        }
        int next_way = path_mgr_->get_way(client_way, 1);
        switch (next_way){
        case switch_way:
        	break;
        case out_way:
        	EthernetII eth = EthernetII()/tuple.inner_.ip_;
        	nat_->nat_out(IPv4Address("10.0.0.167"), sess->uid(), eth);
        	break;
        }


        return true;
    }

    void switcher::on_nat_pkt(const byte_t *data, size_t size) {
    	EthernetII eth(data, size);
    	nat_->nat_in(eth);
        return;
    }

    void switcher::on_switcher_pkt(const byte_t *data, size_t size) {
        return;
    }

} // namespace switcher
