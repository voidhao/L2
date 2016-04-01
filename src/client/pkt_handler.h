#ifndef _LONLIFE_CLIENT_PKT_HANDLER_H_
#define _LONLIFE_CLIENT_PKT_HANDLER_H_

namespace client{

	bool client_pkt_handler(frame_ptr frame);
	bool fill_outer_header(frame_ptr frame, outer_header& outer);
}
#endif /* _LONLIFE_CLIENT_PKT_HANDLER_H_ */
