#ifndef LOGIC_LEVEL_BUFFER_H
#define LOGIC_LEVEL_BUFFER_H

#include <stddef.h>
#include "../yn_t/interface.h"
#include "../LLBufferItem/interface.h"

#define LLBUFFER_LENGTH			100

typedef struct {
	LLBufferItem items[LLBUFFER_LENGTH];
	size_t count;
} LLBuffer;

extern void llb_reset(LLBuffer *self);
extern void llb_addHighTime(LLBuffer *self, unsigned long t);
extern void llb_addLowTime(LLBuffer *self, unsigned long t);
extern yn_t llb_done(LLBuffer *self);
extern unsigned long llb_getHighTimeMin(LLBuffer *self);
extern unsigned long llb_getHighTimeMax(LLBuffer *self);
extern unsigned long llb_getLowTimeMin(LLBuffer *self);
extern unsigned long llb_getLowTimeMax(LLBuffer *self);
extern size_t llb_getLength(LLBuffer *self);

#endif
