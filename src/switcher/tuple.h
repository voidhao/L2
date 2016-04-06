#ifndef _LONLIFE_SWITCHER_TUPLE_H_
#define _LONLIFE_SWITCHER_TUPLE_H_

#include <exception>
#include <memory>
#include <tins/tins.h>
#include <common/limits.h>
#include <common/errors.h>

namespace switcher {

    struct outer_header {
        Tins::EthernetII outer_;

        int fill(const byte_t *data, size_t size) {
            try {
                outer_ = std::move(Tins::EthernetII(data, data + common::outer_len));
            } catch (const std::runtime_error& e) {
                return -1;
            }
            return common::outer_len;
        }
    };

    struct inner_header {
        Tins::EthernetII inner_;

        int fill(const byte_t *data, size_t size) {
            try {
                inner_ = std::move(Tins::EthernetII(data, size));
            } catch (std::runtime_error &e) {
                return -1;
            }
            return size;
        }
    };

    struct vxlan_meta {
        byte_t meta_[common::vxlan_len]{0,0,0,0,0,0,0,0};

        int fill(const byte_t *data, size_t size) {
            if (unlikely(size < common::vxlan_len)) {
                return -1;
            }
            memcpy(meta_, data, common::vxlan_len);
            return common::vxlan_len;
        }
    };

    template<typename MetaData,
            typename Outer = outer_header,
            typename Inner = inner_header>
    struct tuple_t {
        Outer outer_{};
        MetaData meta_{};
        Inner inner_{};

        int fill(const unsigned char *buff, size_t size) {
            int out = outer_.fill(buff, size);
            if (unlikely(out < 0)) {
                return err::OUTER_INVALID;
            }
            int meta = meta_.fill(buff + out, size - out);
            if (unlikely(meta < 0)) {
                return err::META_INVALID;
            }
            int used = out + meta;
            int inner = inner_.fill(buff + used, size - used);
            if (unlikely(inner < 0)) {
                return err::INNER_INVALID;
            }
            return size;
        }

    };

    using vxlan_tuple = tuple_t<vxlan_meta>;
} // namespace switcher
#endif /* _LONLIFE_SWITCHER_TUPLE_H_ */
