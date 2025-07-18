#ifndef READER_SINGLE_CHANNEL_H
#define READER_SINGLE_CHANNEL_H

#include "../logicLevel_t/interface.h"
#include "../biState_t/interface.h"

extern void rsc_begin(const char *description, logicLevel_t (*getLevelFunction)(), unsigned int timeout_ms);
extern biState_t rsc_run();

#endif
