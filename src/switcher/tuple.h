#ifndef _LONLIFE_SWITCHER_TUPLE_H_
#define _LONLIFE_SWITCHER_TUPLE_H_

#include <tins/tins.h>
#include <common/limits.h>

namespace switcher{

struct outer_header{
	Tins::EthernetII eth_;
	Tins::IP 		 ip_;
	Tins::UDP		 udp_;
	static constexpr size_t size(){
		return common::eth_len + common::ip_len + common::udp_len;
	}
	static constexpr size_t min_size(){
		return size();
	}
};

struct inner_header{
	Tins::EthernetII eth_;
	Tins::IP 		 ip_;
	Tins::RawPDU	 payload_;
};

template<typename MetaData>
struct tuple_t{
	outer_header		outer_;
	MetaData			meta_;
	Tins::EthernetII	payload_;

	int operator()(const unsigned char* buff, size_t size){
//		if(size < common::)
		try{

		}catch(...){
			return 1;
		}
		return 0;
	}
};

} // namespace switcher
#endif /* _LONLIFE_SWITCHER_TUPLE_H_ */
