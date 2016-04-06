#include "switcher.h"
#include "tuple.h"
#include "directions.h"
#include <client/client_manager.h>
#include <common/errors.h>

namespace switcher {
    switcher::switcher() :
            client_mgr_{new client::client_manager} {
    }

    switcher::~switcher() {

    }

    void switcher::on_client_pkt(const byte_t *data, size_t size) {
        vxlan_tuple tuple;
        int e = tuple.fill(data, size);
        if (e != 0) {
            return;
        }
        return;
    }

    void switcher::on_nat_pkt(const byte_t *data, size_t size) {
        return;
    }

    void switcher::on_switcher_pkt(const byte_t *data, size_t size) {
        return;
    }
} // namespace switcher