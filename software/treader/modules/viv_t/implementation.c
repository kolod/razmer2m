#include "interface.h"

viv_t intToViv(int v){
	if(v == VALID){
		return (viv_t)v;
	}
	return INVALID;
}

const char *vivToStr(viv_t v){
	switch(v){
		case VALID: return "VALID";
		case INVALID: return "INVALID";
	}
	return "?";
}
