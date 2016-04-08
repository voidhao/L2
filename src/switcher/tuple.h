#ifndef _LONLIFE_SWITCHER_TUPLE_H_
#define _LONLIFE_SWITCHER_TUPLE_H_

#include <exception>
#include <memory>
#include <tins/tins.h>
#include <common/limits.h>
#include <common/errors.h>

namespace switcher {

    struct outer_header {
        Tins::EthernetII eth_;
        Tins::IP 		 ip_;
        Tins::UDP		 udp_;

        int fill(const byte_t *data, size_t size) {
            try {
                eth_ = std::move(Tins::EthernetII(data, data + common::eth_len));
                ip_ = std::move(Tins::IP(data + common::eth_len, common::ip_len));
                udp_ = std::move(Tins::UDP(data + common::eth_len + common::ip_len, common::udp_len));
            } catch (const std::runtime_error& e) {
                return -1;
            }
            return common::outer_len;
        }
        IPv4Address src_addr()const{
        	return ip_.src_addr();
        }
        IPv4Address dst_addr()const{
        	return ip_.dst_addr();
        }
        uint16_t sport()const{
        	return udp_.sport();
        }
        uint16_t dport()const{
        	return udp_.dport();
        }
    };

    struct inner_header {
        Tins::EthernetII eth_;
        Tins::IP		 ip_;

        int fill(const byte_t *data, size_t size) {
            try {
                eth_ = std::move(Tins::EthernetII(data, common::eth_len));
                ip_  = std::move(Tins::IP(data + common::eth_len, size - common::eth_len));
            } catch (std::runtime_error &e) {
                return -1;
            }
            return size;
        }
        IPv4Address src_addr()const{
        	return ip_.src_addr();
        }
        IPv4Address dst_addr()const{
        	return ip_.dst_addr();
        }
    };

    struct vxlan_meta {
        int fill(const byte_t *data, size_t size) {
            if (unlikely(size < common::vxlan_len)) {
                return -1;
            }
            memcpy(this, data, common::vxlan_len);
            return common::vxlan_len;
        }
        uint32_t 	uid()const{
        	return vni();
        }
        uint32_t 	vni()const{
        	int out = 0;
        	memcpy(&out, vni_, 3);
        	return out;
        }
    	uint8_t		flags_{0};
    	uint8_t		rsv1_[3]{0,0,0};
    	uint8_t		vni_[3]{0,0,0};
    	uint8_t		rsv2_{0};
    };

    template<typename MetaData,
            typename Outer = outer_header,
            typename Inner = inner_header>
    struct tuple_t {
        Outer outer_{};
        MetaData meta_{};
        Inner inner_{};

        error_t fill(const unsigned char *buff, size_t size) {
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
            return err::SUCCESS;
        }

    };

    using vxlan_tuple = tuple_t<vxlan_meta>;
} // namespace switcher
#endif /* _LONLIFE_SWITCHER_TUPLE_H_ */
