#ifndef _LONLIFE_SWITCHER_PATH_MANAGE_H_
#define _LONLIFE_SWITCHER_PATH_MANAGE_H_

#include <comm_def.h>
#include "directions.h"
namespace switcher{

class path_manager:noncopyable{
public:
	int get_way(int src, int path)const{
		switch (src){
		case client_way:
			return out_way;
		case out_way:
			return client_way;
		}
	}
private:
};

} // namespace switcher
#endif /* _LONLIFE_SWITCHER_PATH_MANAGE_H_ */
