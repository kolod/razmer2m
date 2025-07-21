#include "interface.h"

void llb_reset(LLBuffer *self){
	for(size_t i = 0; i<LLBUFFER_LENGTH; i++){
		LLBufferItem *item = &self->items[i];
		llbi_reset(item);
	}
	self->count = 0;
}

void llb_addHighTime(LLBuffer *self, unsigned long t){
	if(!(self->count < LLBUFFER_LENGTH)){
		return;
	}
	LLBufferItem *item = &self->items[self->count];
	llbi_addHighTime(item, t);
	if(llbi_done(item) == YES){
		self->count++;
	}
}

void llb_addLowTime(LLBuffer *self, unsigned long t){
	if(!(self->count < LLBUFFER_LENGTH)){
		return;
	}
	LLBufferItem *item = &self->items[self->count];
	llbi_addLowTime(item, t);
	if(llbi_done(item) == YES){
		self->count++;
	}
}

yn_t llb_done(LLBuffer *self){
	if(self->count < LLBUFFER_LENGTH){
		return NO;
	}
	return YES;
}

unsigned long llb_getHighTimeMin(LLBuffer *self){
	unsigned long v = llbi_getHighTime(&self->items[0]);
	for(size_t i = 1; i<LLBUFFER_LENGTH; i++){
		LLBufferItem *item = &self->items[i];
		unsigned long v1 = llbi_getHighTime(item);
		if(v1 < v){
			v = v1;
		}
	}
	return v;
}

unsigned long llb_getHighTimeMax(LLBuffer *self){
	unsigned long v = llbi_getHighTime(&self->items[0]);
	for(size_t i = 1; i<LLBUFFER_LENGTH; i++){
		LLBufferItem *item = &self->items[i];
		unsigned long v1 = llbi_getHighTime(item);
		if(v1 > v){
			v = v1;
		}
	}
	return v;
}

unsigned long llb_getLowTimeMin(LLBuffer *self){
	unsigned long v = llbi_getLowTime(&self->items[0]);
	for(size_t i = 1; i<LLBUFFER_LENGTH; i++){
		LLBufferItem *item = &self->items[i];
		unsigned long v1 = llbi_getLowTime(item);
		if(v1 < v){
			v = v1;
		}
	}
	return v;
}

unsigned long llb_getLowTimeMax(LLBuffer *self){
	unsigned long v = llbi_getLowTime(&self->items[0]);
	for(size_t i = 1; i<LLBUFFER_LENGTH; i++){
		LLBufferItem *item = &self->items[i];
		unsigned long v1 = llbi_getLowTime(item);
		if(v1 > v){
			v = v1;
		}
	}
	return v;
}

size_t llb_getLength(LLBuffer *self){
	return LLBUFFER_LENGTH;
}

