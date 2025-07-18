#ifndef VALUE_DURATION_BUFFER_ITEM_H
#define VALUE_DURATION_BUFFER_ITEM_H

#include <stdint.h>

typedef struct {
	uint8_t value;
	unsigned int duration;
} VDBufferItem;

extern void vdbi_reset(VDBufferItem *self);
extern void vdbi_set(VDBufferItem *self, uint8_t value, unsigned long duration);
extern unsigned int vdbi_getDuration(VDBufferItem *self);
extern uint8_t vdbi_getValue(VDBufferItem *self);

#endif
