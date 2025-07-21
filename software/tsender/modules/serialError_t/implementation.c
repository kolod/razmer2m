#include "interface.h"

serialError_t intToSerialError(int v){
	switch(v){
		case SERIAL_ERROR_NONE:
		case SERIAL_ERROR_CONNECTION:
		case SERIAL_ERROR_DEVICE_DISCONNECTED:
		case SERIAL_ERROR_NO_RESPONSE:
			return (serialError_t)v;
	}
	return SERIAL_ERROR_UNKNOWN;
}

const char *serialErrorToStr(serialError_t v){
	switch(v){
		case SERIAL_ERROR_UNKNOWN:				return "SERIAL_ERROR_UNKNOWN";
		case SERIAL_ERROR_NONE:					return "SERIAL_ERROR_NONE";
		case SERIAL_ERROR_CONNECTION: 			return "SERIAL_ERROR_CONNECTION";
		case SERIAL_ERROR_DEVICE_DISCONNECTED:	return "SERIAL_ERROR_DEVICE_DISCONNECTED";
		case SERIAL_ERROR_NO_RESPONSE:			return "SERIAL_ERROR_NO_RESPONSE";
	}
	return "?";
}
