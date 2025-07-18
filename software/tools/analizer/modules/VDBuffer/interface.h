#ifndef VALUE_DURATION_BUFFER_H
#define VALUE_DURATION_BUFFER_H

#include <stdint.h>
#include <stddef.h>
#include "../logicLevel_t/interface.h"
#include "../yn_t/interface.h"
#include "../VDBufferItem/interface.h"

#define VDBUFFER_LENGTH		300

typedef struct {
	VDBufferItem items[VDBUFFER_LENGTH];
	size_t count;
} VDBuffer;

extern void vdb_reset(VDBuffer *self);
extern void vdb_addItem(VDBuffer *self, uint8_t value, unsigned long duration);
extern yn_t vdb_done(VDBuffer *self);
extern void vdb_print(VDBuffer *self);

#endif
