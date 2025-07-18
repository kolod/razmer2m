#pragma once

#include <stdint.h>

extern uint8_t tcode_getSign(uint8_t code);
extern bool tcode_getError(uint8_t code);
extern void tcode_setError(uint8_t *code);
extern void tcode_clearError(uint8_t *code);
