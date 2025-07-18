#ifndef BUFFER_H
#define BUFFER_H

#include <stdint.h>
#include "../result_t/interface.h"
#include "../yn_t/interface.h"
#include "../BufferBlock/interface.h"

#ifdef MODE_TEST
#define BLOCK_COUNT			30
#else
#define BLOCK_COUNT			7
#endif


typedef struct Buffer_st Buffer;
struct Buffer_st {
	 BufferBlock blocks[BLOCK_COUNT];
	 BufferBlock *wblock;//for writing
	 BufferBlock *nrblock;//next for reading (most resent written block)
	 BufferBlock *rblock;//for reading
	 result_t (*writeNext)(Buffer *, uint8_t );
	 result_t (*readNext)(Buffer *, uint8_t *);
	 result_t (*lockForWriting)(Buffer *);
	 void (*unlockForWriting)(Buffer *);
	 result_t (*lockForReading)(Buffer *);
	 void (*unlockForReading)(Buffer *);
};

extern void buffer_begin(Buffer *self);

extern result_t buffer_writeNext(Buffer *self, uint8_t v);
extern result_t buffer_readNext(Buffer *self, uint8_t *out);

extern result_t buffer_lockForWriting(Buffer *self);
extern void buffer_unlockForWriting(Buffer *self);

extern result_t buffer_lockForReading(Buffer *self);
extern void buffer_unlockForReading(Buffer *self);

extern yn_t buffer_readyForReading(Buffer *self);

#ifdef MODE_TEST
extern void buffer_print(Buffer *self);
#endif

#endif
