#ifndef VIV_T_H
#define VIV_T_H

typedef enum {
	INVALID,
	VALID
} viv_t;

extern viv_t intToViv(int v);
extern const char *vivToStr(viv_t v);

#endif
