#ifndef SERIAL_H
#define SERIAL_H

#include "../result_t/interface.h"

extern void serial_begin(int baud_rate, int config);
extern result_t serial_writeByte(void *v);

#endif
