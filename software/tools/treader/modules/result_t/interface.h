#ifndef RESULT_T_H
#define RESULT_T_H

typedef enum {
	RESULT_UNKNOWN,
	RESULT_SUCCESS,
	RESULT_FAILURE
} result_t;

extern result_t intToResult(int v);
extern const char *resultToStr(result_t v);

#endif
