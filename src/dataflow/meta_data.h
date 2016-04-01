#ifndef _LONLIFE_DATAFLOW_META_DATA_H_
#define _LONLIFE_DATAFLOW_META_DATA_H_

#include <strings.h>

namespace dataflow{

template<typename T>
class meta_data_t;

class __vxlan;
template<>
class meta_data_t<__vxlan>{
public:
	enum{vxlan_len = 8};
	size_t 			size()const{return vxlan_len;};
	const byte_t* 	data()const{return data_;};
	~meta_data_t(){};
	meta_data_t(const byte_t* data){
		memcpy(data_, data, vxlan_len);
	}
private:
	byte_t			data_[vxlan_len];
};


} // dataflow



#endif /* _LONLIFE_DATAFLOW_META_DATA_H_ */
