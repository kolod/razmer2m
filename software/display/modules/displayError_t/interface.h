#ifndef DISPLAY_ERROR_T_H
#define DISPLAY_ERROR_T_H

typedef enum {
	DISPLAY_ERROR_UNKNOWN,
	DISPLAY_ERROR_NONE,
	DISPLAY_ERROR_COMMUNICATION,
	DISPLAY_ERROR_BUFFER_READ
} displayError_t;

extern const char *displayError_toString(displayError_t self);

#endif
