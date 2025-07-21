#ifndef SERIAL_ERROR_T_H
#define SERIAL_ERROR_T_H

typedef enum {
	SERIAL_ERROR_UNKNOWN,
	SERIAL_ERROR_NONE,
	SERIAL_ERROR_CONNECTION,
	SERIAL_ERROR_DEVICE_DISCONNECTED,
	SERIAL_ERROR_NO_RESPONSE
} serialError_t;

extern serialError_t intToSerialError(int v);
extern const char *serialErrorToStr(serialError_t v);

#endif
