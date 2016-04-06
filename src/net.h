#ifndef _LONLIFE_NET_H_
#define _LONLIFE_NET_H_

#include "comm_def.h"


namespace switcher{
class switcher;
}

class lonlife_net{
public:
	bool init();
	bool start();
	void stop();
	bool client_pkt(frame_ptr frame);
	bool switch_pkt(frame_ptr frame);
	bool nat_pkt(const byte_t* data, size_t size);
public:
	lonlife_net();
	~lonlife_net();
private:
	std::unique_ptr<switcher::switcher> switcher_;
};


#endif /* _LONLIFE_NET_H_ */
