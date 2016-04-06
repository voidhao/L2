#include "switcher.h"
#include "tuple.h"
#include "directions.h"
#include <client/client_manager.h>
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
        client_mgr_->client_tuple(tuple);
        return true;
    }

    void switcher::on_nat_pkt(const byte_t *data, size_t size) {
        return;
    }

    void switcher::on_switcher_pkt(const byte_t *data, size_t size) {
        return;
    }

} // namespace switcher
