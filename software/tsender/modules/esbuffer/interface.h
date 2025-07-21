#ifndef ESBUFFER_H
#define ESBUFFER_H

#include <stddef.h>
#include <stdint.h>
#include "../yn_t/interface.h"
#include "../result_t/interface.h"

extern void esbuffer_reset();
extern void esbuffer_changeData();
extern void esbuffer_changeNextItemError();
extern uint8_t esbuffer_getItemSign(int item_index);
extern yn_t esbuffer_getItemError(int item_index);
extern uint8_t esbuffer_getItemCode(int item_index);
extern result_t esbuffer_setItemByCode(uint8_t code, int item_index);
extern size_t esbuffer_getLength();
extern void esbuffer_print();

#endif
