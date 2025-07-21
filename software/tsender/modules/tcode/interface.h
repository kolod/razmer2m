#ifndef TRANSFER_CODE_H
#define TRANSFER_CODE_H

#include <stdint.h>
#include "../yn_t/interface.h"

extern uint8_t tcode_getSign(uint8_t code);
extern yn_t tcode_getError(uint8_t code);
extern void tcode_setError(uint8_t *code);
extern void tcode_clearError(uint8_t *code);

#endif
