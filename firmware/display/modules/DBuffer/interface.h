#ifndef DISPLAY_BUFFER_H
#define DISPLAY_BUFFER_H

#include <stdint.h>
#include "../result_t/interface.h"
#include "../yn_t/interface.h"
#include "../razmer2m1104.h"

typedef struct DBuffer_st DBuffer;
struct DBuffer_st {
	uint8_t codes[RAZMER2M_SIGNS_COUNT];
	uint8_t segments[RAZMER2M_SIGNS_COUNT];
	void (*changeError)(DBuffer *);
	yn_t fresh;
};

extern void dbuffer_begin(DBuffer *self);
extern yn_t dbuffer_hasNewData(DBuffer *self);
extern void dbuffer_changeError(DBuffer *self);
extern result_t dbuffer_getSegmentFromColumn(DBuffer *self, int index, uint8_t *b1, uint8_t *b2, uint8_t *b3, uint8_t *b4);
extern result_t dbuffer_setCodeToColumn(DBuffer *self, int index, uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4);
#ifdef MODE_TEST
extern void dbuffer_print(DBuffer *self);
#endif

#endif
