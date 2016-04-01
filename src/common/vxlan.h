#ifndef _LONLIFE_COMMON_VXLAN_H_
#define _LONLIFE_COMMON_VXLAN_H_

namespace common {

struct vxlan_hdr {
	uint8_t		flags_;
	uint8_t		rsv1_[3];
	uint8_t		vni_[3];
	uint8_t		rsv2_;
};
} // namespace common

#endif /* _LONLIFE_COMMON_VXLAN_H_ */
