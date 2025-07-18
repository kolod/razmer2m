#pragma message ( "\n\n \
	Reader: imitation.\n \
" )

#include "../../razmer2m1104.h"
#include "../../asciiBcd/interface.h"
#include "../../Ton/interface.h"
#include "../../connection.h"
#include "../../TBuffer/interface.h"
#include "../../debug/interface.h"
#ifdef MODE_TEST
#include <stdio.h>
#endif
static Ton change_timern;
static Ton *change_timer = &change_timern;

static Ton send_timern;
static Ton *send_timer = &send_timern;

static Ton error_timern;
static Ton *error_timer = &error_timern;

static TBuffer tbuffern;
static TBuffer *tbuffer = &tbuffern;

static void readerRun_DUMMY(Buffer *buffer);
static void readerRun_START(Buffer *buffer);
static void readerRun_SEND(Buffer *buffer);

static void (*readerRun)(Buffer *buffer) = readerRun_DUMMY;

static void sendDataToBuffer(Buffer *buffer){
	if(buffer_lockForWriting(buffer) == RESULT_SUCCESS){
		for(int i=0; i<RAZMER2M_SIGNS_COUNT; i++){
			buffer_writeNext(buffer, tbuffer_getItemCode(tbuffer, i));
		}
		buffer_unlockForWriting(buffer);
	}	
}

static void readerRun_DUMMY(Buffer *buffer){
	;
}

static void readerRun_START(Buffer *buffer){
	tbuffer_reset(tbuffer);
	ton_setInterval(send_timer, 20);
	ton_reset(send_timer);
	ton_setInterval(change_timer, 100);
	ton_reset(change_timer);
	ton_setInterval(error_timer, 1000);
	ton_reset(error_timer);
	readerRun = readerRun_SEND;
}

static void readerRun_SEND(Buffer *buffer){
	if(tonr(send_timer)){
		sendDataToBuffer(buffer);
	}
	if(tonr(change_timer)){
		tbuffer_changeData(tbuffer);
	}
	if(tonr(error_timer)){
		tbuffer_changeError(tbuffer);debug_switch();
	}
}

void reader_begin(){
#ifdef MODE_TEST
	printf("hellow from reader immitation\n");
#endif
	readerRun = readerRun_START;
}

void reader_run(Buffer *buffer){
	readerRun(buffer);
}
