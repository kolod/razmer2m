#include "interface.h"

#include <stdio.h>
#include "../appSerial/interface.h"
#include "../esbuffer/interface.h"

void reader_begin(){
	appSerial_begin();
}

static void clearScr(){
	printf("\033c");
}

void reader_run(){
	bsfState_t r = appSerial_readInBuffer();
	if(r == SUCCESS){
		clearScr();
		esbuffer_print();
	} else {
		clearScr();
		printf("ERROR: ");
		appSerialError_t e = appSerial_getError();
		switch(e){
			case APP_SERIAL_ERROR_NO_DATA: printf("no data.\n"); break;
			case APP_SERIAL_ERROR_INVALID_DATA: printf("invalid data.\n"); break;
			case APP_SERIAL_ERROR_DEVICE_DISCONNECTED: printf("device disconnected.\n"); break;
			case APP_SERIAL_ERROR_NONE: printf("none.\n"); break;
			default: printf("unknown.\n"); break;
		}
	}
}

void reader_end(){
	appSerial_end();
}
