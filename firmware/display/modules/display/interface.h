#ifndef DISPLAY_H
#define DISPLAY_H

#include "../Buffer/interface.h"

extern void display_begin(unsigned int communication_timeout_ms);
extern void display_run(Buffer *buffer);


#endif
