#ifndef _LONLIFE_COMMON_ERRORS_H_
#define _LONLIFE_COMMON_ERRORS_H_

namespace err{

	using err_t	= const int;

	err_t	OUTER_INVALID	= 100;
	err_t	INNER_INVALID	= 101;
	err_t	META_INVALID	= 102;

	err_t	UNKOWN	= 9999999;
/*
 * User Error
 */


	static const char* msg(int err){
		switch(err){
		case OUTER_INVALID: return "Client Outer Data Invalid";
		case INNER_INVALID: return "Client Inner Data Invalid";
		case META_INVALID:  return "Client Meta Data Invalid";

		default: return "UNKOWN ERROR";
		}
	}

}
#endif /* ERRORS_H_ */
