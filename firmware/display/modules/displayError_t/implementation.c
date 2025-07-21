#include "interface.h"

const char *displayError_toString(displayError_t self){
	switch(self){
		case DISPLAY_ERROR_NONE:			return "Eno";
		case DISPLAY_ERROR_COMMUNICATION:	return "Eco";
		case DISPLAY_ERROR_BUFFER_READ:		return "Erb";
		default: break;
	}
	return "E?";
}
