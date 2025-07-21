#ifndef READER_MULTY_CHANNEL_H
#define READER_MULTY_CHANNEL_H

#include "../biState_t/interface.h"

extern void rmc_begin(unsigned int timeout_ms);
extern biState_t rmc_run();

#endif
