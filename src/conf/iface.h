#ifndef IFACE_H_
#define IFACE_H_

#include <comm_def.h>

namespace conf{

string_vector read_iface();

	struct iface{
		std::string name_;
		IPv4Address gateway_;
	};

	using ifaces = std::vector<iface>;
	ifaces read_ifaces();
} // namespace conf
#endif /* IFACE_H_ */
