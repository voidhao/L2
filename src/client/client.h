#ifndef _LONLIFE_CLIENT_H_
#define _LONLIFE_CLIENT_H_
#include <comm_def.h>
#include <client/session.h>
#include <client/client_manager.h>

struct outer_header{
	IPv4Address	src_;
	IPv4Address dest_;
	uint16_t	sport_;
	uint16_t	dport_;
	VxLAN		vxlan_;
	friend std::ostream& operator<<(std::ostream& output, const outer_header& outer){
		char buff[256];
		snprintf(buff, 256, "%s:%d->%s:%d",
				outer.src_.to_string().c_str(),
				(outer.sport_),
				outer.dest_.to_string().c_str(),
				(outer.dport_));
		output << buff ;
		return output;
	}
};



#include "pkt_handler.h"

#endif /* _LONLIFE_CLIENT_H_ */
