#include "interface.h"

result_t intToResult(int v){
	switch(v){
		case RESULT_SUCCESS:
		case RESULT_FAILURE:
			return (result_t) v;
	}
	return RESULT_UNKNOWN;
}

const char *resultToStr(result_t v){
	switch(v){
		case RESULT_SUCCESS: return "SUCCESS";
		case RESULT_FAILURE: return "FAILURE";
		default: break;
	}
	return "?";
}
