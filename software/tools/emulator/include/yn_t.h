#pragma once

typedef enum {
	NO,
	YES
} yn_t;

extern yn_t intToYn(int v);
extern const char *ynToStr(yn_t v);
