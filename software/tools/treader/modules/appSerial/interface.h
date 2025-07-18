#ifndef APP_SERIAL_H
#define APP_SERIAL_H

#include "../bsfState_t/interface.h"
#include "../appSerialError_t/interface.h"

extern void appSerial_begin();
extern bsfState_t appSerial_readInBuffer();
extern appSerialError_t appSerial_getError();
extern void appSerial_end();

#endif
