#include "interface.h"
#include <stddef.h>
#ifdef MODE_TEST
#include <stdio.h>
#endif
static result_t writeNext_DUMMY(BufferBlock *self, uint8_t v);
static result_t writeNext_ACTIVE(BufferBlock *self, uint8_t v);

static result_t readNext_DUMMY(BufferBlock *self, uint8_t *out);
static result_t readNext_ACTIVE(BufferBlock *self, uint8_t *out);

static void resetSymbols(BufferBlock *self){
	for(int i=0; i<BUFFER_BLOCK_SYMBOL_COUNT; i++){
		self->symbol[i] = 0;
	}
}

void bufferBlock_begin(BufferBlock *self){
	resetSymbols(self);
	self->i = 0;
	self->writeNext = writeNext_DUMMY;
	self->readNext = readNext_DUMMY;
	self->mode = BUFFER_BLOCK_MODE_FREE;
	self->next = NULL;
}

static result_t writeNext_ACTIVE(BufferBlock *self, uint8_t v){
	int ind = self->i;
	if(ind < BUFFER_BLOCK_SYMBOL_COUNT){
		self->symbol[ind] = v;
		self->i++;
		return RESULT_SUCCESS;
	}
	self->writeNext = writeNext_DUMMY;
	return RESULT_FAILURE;
}

static result_t writeNext_DUMMY(BufferBlock *self, uint8_t v){
	return RESULT_FAILURE;
}

result_t bufferBlock_writeNext(BufferBlock *self, uint8_t v){
	return self->writeNext(self, v);
}

static result_t readNext_ACTIVE(BufferBlock *self, uint8_t *out){
	int ind = self->i;
	if(ind < BUFFER_BLOCK_SYMBOL_COUNT){
		*out = self->symbol[ind];
		self->i++;
		return RESULT_SUCCESS;
	}
	self->readNext = readNext_DUMMY;
	return RESULT_FAILURE;
}

static result_t readNext_DUMMY(BufferBlock *self, uint8_t *out){
	return RESULT_FAILURE;
}

result_t bufferBlock_readNext(BufferBlock *self, uint8_t *out){
	return self->readNext(self, out);
}

result_t bufferBlock_lockForWriting(BufferBlock *self){
	if(self->mode == BUFFER_BLOCK_MODE_FREE || self->mode == BUFFER_BLOCK_MODE_EXPIRED){
		resetSymbols(self);
		self->i = 0;
		self->mode = BUFFER_BLOCK_MODE_WRITE;
		self->writeNext = writeNext_ACTIVE;
		self->readNext = readNext_DUMMY;
		return RESULT_SUCCESS;
	}
	return RESULT_FAILURE;
}

result_t bufferBlock_lockForReading(BufferBlock *self){
	if(self->mode == BUFFER_BLOCK_MODE_FREE){
		self->i = 0;
		self->mode = BUFFER_BLOCK_MODE_READ;
		self->writeNext = writeNext_DUMMY;
		self->readNext = readNext_ACTIVE;
		return RESULT_SUCCESS;
	}
	return RESULT_FAILURE;
}

void bufferBlock_unlock(BufferBlock *self){
	if(self->mode != BUFFER_BLOCK_MODE_FREE){
		self->writeNext = writeNext_DUMMY;
		self->readNext = readNext_DUMMY;
		if(self->mode == BUFFER_BLOCK_MODE_READ){
			self->mode = BUFFER_BLOCK_MODE_EXPIRED;
		} else {
			self->mode = BUFFER_BLOCK_MODE_FREE;
		}
	}
}

void bufferBlock_setNext(BufferBlock *self, BufferBlock *next){
	self->next = next;
}

BufferBlock *bufferBlock_getNext(BufferBlock *self){
	return self->next;
}

yn_t bufferBlock_isFreeForWriting(BufferBlock *self){
	if(self->mode == BUFFER_BLOCK_MODE_FREE || self->mode == BUFFER_BLOCK_MODE_EXPIRED){
		return YES;
	}
	return NO;
}

yn_t bufferBlock_readyForReading(BufferBlock *self){
	if(self->mode == BUFFER_BLOCK_MODE_FREE){
		return YES;
	}
	return NO;
}

#ifdef MODE_TEST
void bufferBlock_print(BufferBlock *self){
	switch(self->mode){
		case BUFFER_BLOCK_MODE_FREE:	printf("F"); return;
		case BUFFER_BLOCK_MODE_WRITE:	printf("W"); return;
		case BUFFER_BLOCK_MODE_READ:	printf("R"); return;
		case BUFFER_BLOCK_MODE_EXPIRED: printf("E"); return;
		default: break;
	}
	printf("?");
}
#endif

