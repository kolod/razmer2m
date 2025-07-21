#pragma message ( "\n\n \
	Reader: real.\n \
" )
#include "../../razmer2m1104.h"
#include "../../connection.h"
#include "../../Ton/interface.h"
#ifdef MODE_TEST
#include <stdio.h>
#endif
#include "../../gpio/interface.h"
#include "../../debug/interface.h"
#define READER_TIMEOUT_MS			50
#define SEND_ERROR_INTERVAL_MS		20
Ton error_timern;
Ton *error_timer = &error_timern;
Ton send_error_timern;
Ton *send_error_timer = &send_error_timern;

static void readData_DUMMY(Buffer *buffer, yn_t timeout);
static void readData_A7NotActive(Buffer *buffer, yn_t timeout);
static void readData_A7Active(Buffer *buffer, yn_t timeout);
static void readData_A7NotActive2(Buffer *buffer, yn_t timeout);
static void readData_B0(Buffer *buffer, yn_t timeout);
static void readData_B1(Buffer *buffer, yn_t timeout);
static void readData_B2(Buffer *buffer, yn_t timeout);
static void readData_B3(Buffer *buffer, yn_t timeout);
static void readData_B4(Buffer *buffer, yn_t timeout);
static void readData_ERROR_SIGNALS(Buffer *buffer, yn_t timeout);

static void (*readData)(Buffer *, yn_t) = readData_DUMMY;

static int count = 0;

static void readCode(Buffer *buffer){
	buffer_writeNext(buffer, gpio_read()); count++;
}

static void setError(Buffer *buffer){
	buffer_writeAll(buffer, CONNECTION_ERROR_MASK_1);
}


static void readData_DUMMY(Buffer *buffer, yn_t timeout){
	;
}

static void readData_ERROR_SIGNALS(Buffer *buffer, yn_t timeout){debug_echo();
	if(tonr(send_error_timer)){//debug_echo();
		if(buffer_lockForWriting(buffer) != RESULT_SUCCESS){
			setError(buffer);
			buffer_unlockForWriting(buffer);
		}
	}
}

static void readData_A7NotActive(Buffer *buffer, yn_t timeout){
	if(gpio_A7isActive() == NO){
		ton_reset(error_timer);
		readData = readData_A7Active;
		return;
	}
}

static void readData_A7Active(Buffer *buffer, yn_t timeout){
	if(timeout == YES){
		readData = readData_ERROR_SIGNALS;
		return;
	}
	if(gpio_A7isActive() == YES){
		if(buffer_lockForWriting(buffer) != RESULT_SUCCESS){
			readData = readData_A7NotActive;
			return;
		}
		count = 0;
		readData = readData_A7NotActive2;
		return;
	}
}

static void readData_A7NotActive2(Buffer *buffer, yn_t timeout){
	if(timeout == YES){
		buffer_unlockForWriting(buffer);
		readData = readData_ERROR_SIGNALS;
		return;
	}
	if(gpio_A7isActive() == NO){
		readData = readData_B0;
		return;
	}
}

static void readData_B0(Buffer *buffer, yn_t timeout){
	if(timeout == YES){
		buffer_unlockForWriting(buffer);
		readData = readData_ERROR_SIGNALS;
		return;
	}
	if(gpio_B0isActive() == YES){
		readData = readData_B1;
		return;
	}
}

static void readData_B1(Buffer *buffer, yn_t timeout){
	if(timeout == YES){
		buffer_unlockForWriting(buffer);
		readData = readData_ERROR_SIGNALS;
		return;
	}
	if(gpio_B1isActive() == YES){
		readCode(buffer);
		readData = readData_B2;
		return;
	}
}

static void readData_B2(Buffer *buffer, yn_t timeout){
	if(timeout == YES){
		buffer_unlockForWriting(buffer);
		readData = readData_ERROR_SIGNALS;
		return;
	}
	if(gpio_B2isActive() == YES){
		readCode(buffer);
		readData = readData_B3;
		return;
	}
}

static void readData_B3(Buffer *buffer, yn_t timeout){
	if(timeout == YES){
		buffer_unlockForWriting(buffer);
		readData = readData_ERROR_SIGNALS;
		return;
	}
	if(gpio_B3isActive() == YES){
		readCode(buffer);
		readData = readData_B4;
		return;
	}
}

static void readData_B4(Buffer *buffer, yn_t timeout){
	if(timeout == YES){
		buffer_unlockForWriting(buffer);
		readData = readData_ERROR_SIGNALS;
		return;
	}
	if(gpio_B4isActive() == YES){
		readCode(buffer);
		if(count < RAZMER2M_SIGNS_COUNT){
			readData = readData_B0;
		} else {
			buffer_unlockForWriting(buffer);
			readData = readData_A7NotActive;
		}
		return;
	}
}

void reader_begin(){
#ifdef MODE_TEST
	printf("hellow from real reader\n");
#endif
	ton_setInterval(error_timer, READER_TIMEOUT_MS);
	ton_reset(error_timer);
	ton_setInterval(send_error_timer, SEND_ERROR_INTERVAL_MS);
	ton_reset(send_error_timer);
	readData = readData_A7NotActive;
}

void reader_run(Buffer *buffer){
	yn_t timeout = NO;
	if(ton(error_timer)){
		timeout = YES;
	}
	readData(buffer, timeout);
}
