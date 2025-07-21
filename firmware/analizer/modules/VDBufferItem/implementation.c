#include "interface.h"

void vdbi_reset(VDBufferItem *self){
	self->value = 0;
	self->duration = 0;
}

void vdbi_set(VDBufferItem *self, uint8_t value, unsigned long duration){
	self->value = value;
	self->duration = duration;
}

unsigned int vdbi_getDuration(VDBufferItem *self){
	return self->duration;
}

uint8_t vdbi_getValue(VDBufferItem *self){
	return self->value;
}
