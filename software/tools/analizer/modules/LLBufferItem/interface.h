#ifndef LOGIC_LEVEL_BUFFER_ITEM_H
#define LOGIC_LEVEL_BUFFER_ITEM_H

#include "../yn_t/interface.h"

typedef struct {
	unsigned long high_time;
	unsigned long low_time;
	int count;
} LLBufferItem;

extern void llbi_reset(LLBufferItem *self);
extern void llbi_addHighTime(LLBufferItem *self, unsigned long t);
extern void llbi_addLowTime(LLBufferItem *self, unsigned long t);
extern unsigned long llbi_getHighTime(LLBufferItem *self);
extern unsigned long llbi_getLowTime(LLBufferItem *self);
extern yn_t llbi_done(LLBufferItem *self);

#endif
