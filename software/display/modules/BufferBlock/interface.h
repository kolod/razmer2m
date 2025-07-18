#ifndef BUFFER_BLOCK_H
#define BUFFER_BLOCK_H

#include <stdint.h>
#include "../result_t/interface.h"
#include "../yn_t/interface.h"
#include "../bufferBlockMode_t/interface.h"
#include "../razmer2m1104.h"

#define BUFFER_BLOCK_SYMBOL_COUNT		 RAZMER2M_SIGNS_COUNT

typedef struct BufferBlock_st BufferBlock;
struct BufferBlock_st {
	uint8_t symbol[BUFFER_BLOCK_SYMBOL_COUNT];
	int i;
	result_t (*writeNext)(BufferBlock *, uint8_t);
	result_t (*readNext)(BufferBlock *, uint8_t *);
	bufferBlockMode_t mode;
	BufferBlock *next;
};

extern void bufferBlock_begin(BufferBlock *self);
extern result_t bufferBlock_writeNext(BufferBlock *self, uint8_t v);
extern result_t bufferBlock_readNext(BufferBlock *self, uint8_t *out);
extern result_t bufferBlock_lockForWriting(BufferBlock *self);
extern result_t bufferBlock_lockForReading(BufferBlock *self);
extern void bufferBlock_unlock(BufferBlock *self);
extern yn_t bufferBlock_isFreeForWriting(BufferBlock *self);
extern yn_t bufferBlock_readyForReading(BufferBlock *self);
extern void bufferBlock_setNext(BufferBlock *self, BufferBlock *next);
extern BufferBlock *bufferBlock_getNext(BufferBlock *self);

#ifdef MODE_TEST
extern void bufferBlock_print(BufferBlock *self);
#endif

#endif
