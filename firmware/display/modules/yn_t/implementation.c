#include "interface.h"

yn_t intToYn(int v){
	if(v == YES){
		return (yn_t)v;
	}
	return NO;
}

const char *ynToStr(yn_t v){
	switch(v){
		case YES: return "YES";
		case NO: return "NO";
	}
	return "?";
}
