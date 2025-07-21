#ifndef TRANSFER_BUFFER_H
#define TRANSFER_BUFFER_H

#include <stdint.h>
#include "../razmer2m1104.h"
#include "../yn_t/interface.h"

typedef struct tbuffer_st {
	uint8_t items[RAZMER2M_SIGNS_COUNT];
	int error_index;
} TBuffer;

extern int tbuffer_getCount(TBuffer *self);
extern void tbuffer_reset(TBuffer *self);
extern void tbuffer_changeData(TBuffer *self);
extern void tbuffer_changeError(TBuffer *self);
extern uint8_t tbuffer_getItemSign(TBuffer *self, int item_index);
extern yn_t tbuffer_getItemError(TBuffer *self, int item_index);
extern uint8_t tbuffer_getItemCode(TBuffer *self, int item_index);

#endif
