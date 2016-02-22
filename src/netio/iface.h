#ifndef _NETIO_IFACE_H_
#define _NETIO_IFACE_H_

struct nm_desc;

namespace netio{

struct iface{
	nm_desc*	desc_{nullptr};
};

}

#endif /* IFACE_H_ */
