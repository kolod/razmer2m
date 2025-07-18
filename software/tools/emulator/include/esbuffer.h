#pragma once

#include <stdint.h>
#include "yn_t.h"

extern void esbuffer_reset();
extern void esbuffer_changeData();
extern void esbuffer_changeNextItemError();
extern uint8_t esbuffer_getItemSign(int item_index);
extern yn_t esbuffer_getItemError(int item_index);
