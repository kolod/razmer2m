#include "interface.h"
#include <stddef.h>
#ifdef MODE_TEST
#include <stdio.h>
#endif
static result_t writeNext_DISABLED(Buffer *self, uint8_t v);
static result_t writeNext_ENABLED(Buffer *self, uint8_t v);
static result_t readNext_DISABLED(Buffer *self, uint8_t *out);
static result_t readNext_ENABLED(Buffer *self, uint8_t *out);
static result_t lockForWriting_DISABLED(Buffer *self);
static result_t lockForWriting_ENABLED(Buffer *self);
static void unlockForWriting_DISABLED(Buffer *self);
static void unlockForWriting_ENABLED(Buffer *self);
static result_t lockForReading_DISABLED(Buffer *self);
static result_t lockForReading_ENABLED(Buffer *self);
static void unlockForReading_DISABLED(Buffer *self);
static void unlockForReading_ENABLED(Buffer *self);

void buffer_begin(Buffer *self){
	for(int i=0; i<BLOCK_COUNT; i++){
		bufferBlock_begin(&self->blocks[i]);
	}
	for(int i=0; i<BLOCK_COUNT - 1; i++){
		bufferBlock_setNext(&self->blocks[i], &self->blocks[i+1]);
	}
	bufferBlock_setNext(&self->blocks[BLOCK_COUNT-1], &self->blocks[0]);
	self->wblock = NULL;
	self->nrblock = NULL;
	self->rblock = NULL;
	self->lockForWriting = lockForWriting_ENABLED;
	self->unlockForWriting = unlockForWriting_DISABLED;
	self->lockForReading = lockForReading_ENABLED;
	self->unlockForReading = unlockForReading_DISABLED;
}

static result_t writeNext_DISABLED(Buffer *self, uint8_t v){
	return RESULT_FAILURE;
}

static result_t writeNext_ENABLED(Buffer *self, uint8_t v){
	return bufferBlock_writeNext(self->wblock, v);
}

result_t buffer_writeNext(Buffer *self, uint8_t v){
	return self->writeNext(self, v);
}

static result_t readNext_DISABLED(Buffer *self, uint8_t *out){
	return RESULT_FAILURE;
}

static result_t readNext_ENABLED(Buffer *self, uint8_t *out){
	return bufferBlock_readNext(self->rblock, out);
}

result_t buffer_readNext(Buffer *self, uint8_t *out){
	return self->readNext(self, out);
}

static BufferBlock *getNextWblock(Buffer *self){
	int count = 0;
	if(self->wblock == NULL){
		return &self->blocks[0];
	}
	BufferBlock *b = self->wblock;
	while(1){
		b = b->next;
		count++;
		if(b == NULL){
			break;
		}
		if(count >= BLOCK_COUNT){
			break;
		}
		if(bufferBlock_isFreeForWriting(b) == YES){
			return b;
		}
	}
	return NULL;
}

static result_t lockForWriting_DISABLED(Buffer *self){
	return RESULT_FAILURE;
}

static result_t lockForWriting_ENABLED(Buffer *self){
	BufferBlock *next_block = getNextWblock(self);
	if(next_block == NULL){
		return RESULT_FAILURE;
	}
	if(bufferBlock_lockForWriting(next_block) != RESULT_SUCCESS){
		return RESULT_FAILURE;
	}
	self->wblock = next_block;
	self->writeNext = writeNext_ENABLED;
	self->unlockForWriting = unlockForWriting_ENABLED;
	self->lockForWriting = lockForWriting_DISABLED;
	return RESULT_SUCCESS;
}

result_t buffer_lockForWriting(Buffer *self){
	return self->lockForWriting(self);
}

static void unlockForWriting_DISABLED(Buffer *self){
	;
}

static void unlockForWriting_ENABLED(Buffer *self){
	bufferBlock_unlock(self->wblock);
	self->nrblock = self->wblock;// now reader can read from this block
	self->writeNext = writeNext_DISABLED;
	self->lockForWriting = lockForWriting_ENABLED;
	self->unlockForWriting = unlockForWriting_DISABLED;
}

void buffer_unlockForWriting(Buffer *self){
	self->unlockForWriting(self);
}

static result_t lockForReading_DISABLED(Buffer *self){
	return RESULT_FAILURE;
}

static result_t lockForReading_ENABLED(Buffer *self){
	if(self->nrblock == NULL){
		return RESULT_FAILURE;
	}
	if(bufferBlock_lockForReading(self->nrblock) != RESULT_SUCCESS){
		return RESULT_FAILURE;
	}
	self->rblock = self->nrblock;
	self->readNext = readNext_ENABLED;
	self->unlockForReading = unlockForReading_ENABLED;
	self->lockForReading = lockForReading_DISABLED;
	return RESULT_SUCCESS;
}

result_t buffer_lockForReading(Buffer *self){
	return self->lockForReading(self);
}

yn_t buffer_readyForReading(Buffer *self){
	if(self->nrblock == NULL){
		return NO;
	}
	return bufferBlock_readyForReading(self->nrblock);
}

static void unlockForReading_DISABLED(Buffer *self){
	;
}
	
static void unlockForReading_ENABLED(Buffer *self){
	bufferBlock_unlock(self->wblock);
	self->readNext = readNext_DISABLED;
	self->lockForReading = lockForReading_ENABLED;
	self->unlockForReading = unlockForReading_DISABLED;
}

void buffer_unlockForReading(Buffer *self){
	self->unlockForReading(self);
}

#ifdef MODE_TEST
static void printPointer(Buffer *self, const char *name, BufferBlock *pointer){
	printf("|%7s|", name);
	for(int i=0; i<BLOCK_COUNT; i++){
		BufferBlock *block = &self->blocks[i];
		if(block == pointer){
			printf("^");
		} else {
			printf(" ");
		}
	}
	printf("|\n");
}

static void printBlocks(Buffer *self, const char *name){
	printf("|%7s|", name);
	for(int i=0; i<BLOCK_COUNT; i++){
		BufferBlock *block = &self->blocks[i];
		bufferBlock_print(block);
	}
	printf("|\n");
}

static void printLine(Buffer *self){
	printf("+-------+");
	for(int i=0; i<BLOCK_COUNT; i++){
		printf("-");
	}
	printf("+\n");
}

void buffer_print(Buffer *self){
	printLine(self);
	printBlocks(self, "blocks");
	printPointer(self, "write", self->wblock);
	printPointer(self, "nread", self->nrblock);
	printPointer(self, "read", self->rblock);
	printLine(self);
}
#endif
