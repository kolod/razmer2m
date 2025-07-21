#include "interface.h"

#define LLBI_COUNT_MAX		2

void llbi_reset(LLBufferItem *self){
	self->high_time = 0;
	self->low_time = 0;
	self->count = 0;
}

void llbi_addHighTime(LLBufferItem *self, unsigned long t){
	if(self->count < LLBI_COUNT_MAX){
		self->high_time = t;
		self->count++;
	}
}

void llbi_addLowTime(LLBufferItem *self, unsigned long t){
	if(self->count < LLBI_COUNT_MAX){
		self->low_time = t;
		self->count++;
	}
}

unsigned long llbi_getHighTime(LLBufferItem *self){
	return self->high_time;
}

unsigned long llbi_getLowTime(LLBufferItem *self){
	return self->low_time;
}

yn_t llbi_done(LLBufferItem *self){
	if(self->count < LLBI_COUNT_MAX){
		return NO;
	}
	return YES;
}
