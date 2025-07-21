#ifndef SERIAL_H
#define SERIAL_H

#include <stddef.h>

#include "../result_t/interface.h"
#include "../viv_t/interface.h"
#include "../yn_t/interface.h"
#include "../serialError_t/interface.h"

#define SERIAL_CONFIG_STRLEN	3

extern viv_t serial_checkBaud(int baud);
extern viv_t serial_checkConfig(const char *config);
extern int serial_open(const char *device, const int baud, const char *config);
extern result_t serial_begin(int *fd, const char *device, const int baud, const char *config);
extern void serial_printOptions (const int fd);
extern result_t serial_puts(const int fd, char *str);
extern yn_t serial_canRead(int fd, int timeout_ms);
extern yn_t serial_canWrite(int fd, int timeout_ms);
extern yn_t serial_alive(int fd);
extern size_t serial_read(int fd, void *vbuf, size_t buf_size);
extern size_t serial_readUntil(int fd, void *vbuf, size_t buf_size, char end);
extern size_t serial_readFromTo(int fd, void *vbuf, size_t buf_size, char start, char end);
extern void serial_readAll(int fd);
extern serialError_t serial_sendStr(int fd, const char *str);
extern serialError_t serial_readResponse(int fd, void *vbuf, size_t buf_len, char start, char end, int timeout_ms);
extern void serial_flush(const int fd);

#endif

