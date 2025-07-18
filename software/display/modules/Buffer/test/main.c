#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include "../../result_t/interface.h"
#include "../../yn_t/interface.h"
#include "../interface.h"

#include "../../yn_t/implementation.c"
#include "../../bufferBlockMode_t/interface.h"
#include "../../BufferBlock/implementation.c"
#include "../../Buffer/implementation.c"
#include "./modules/stime_t/implementation.c"
#include "./modules/Timer/implementation.c"

Buffer buffern;
Buffer *buffer = &buffern;

const char *data = "abcdefghijklmnopqrstuvwxyz12";

Timer read_timern;
Timer *read_timer = &read_timern;

Timer write_timern;
Timer *write_timer = &write_timern;

int writeToBuffer(){
	if(!timer_done(write_timer)) return 0;
	timer_reset(write_timer);
	if(buffer_lockForWriting(buffer) == RESULT_SUCCESS){
		size_t len = strlen(data);
		for(size_t i=0; i<len; i++){
			uint8_t v = data[i];
			if(buffer_writeNext(buffer, v) != RESULT_SUCCESS){
				printf("failed to write value:%hhd, index:%zu\n", v, i);
				continue;
			}
		}
		buffer_unlockForWriting(buffer);
		printf(">>>>>>>>>after WRITE:\n");
		buffer_print(buffer);
		return 1;
	}
	printf("write lock failed\n");
	return 0;
}

int readFromBuffer(){
	if(!timer_done(read_timer)) return 0;
	timer_reset(read_timer);
	if(buffer_lockForReading(buffer) == RESULT_SUCCESS){
		size_t len = strlen(data);
		for(size_t i=0; i<len; i++){
			uint8_t dv = data[i];
			uint8_t bv;
			if(buffer_readNext(buffer, &bv) != RESULT_SUCCESS){
				printf("failed to read at index:%zu\n", i);
				continue;
			}
			if(bv != dv){
				printf("invalid read: from buffer:%hhu, expected:%hhu\n", bv, dv);
				continue;
			}
		}
		buffer_unlockForReading(buffer);
		printf("         after READ:\n");
		buffer_print(buffer);
		return 1;
	}
	printf("read lock failed\n");
	return 0;
}

void test(int write_interval_us, int read_interval_us, int write_count_max){
	timer_setInterval(read_timer, usToStime(read_interval_us));
	timer_reset(read_timer);
	timer_setInterval(write_timer, usToStime(write_interval_us));
	timer_reset(write_timer);
	for(int write_count = 0; write_count < write_count_max;){
		int w = writeToBuffer();
		readFromBuffer();
		write_count += w;
		//printf("%d ", write_count);
	}
}

void fastWriting(){
	printf("-----------------WRITING IS FASTER THAN READING-----------------\n");
	test(1000, 5000, 60);
	printf("----------------------------------------------------------------\n\n\n");
}

void fastReading(){
	printf("-----------------READING IS FASTER THAN WRITING-----------------\n");
	test(5000, 1000, 60);
	printf("----------------------------------------------------------------\n\n\n");
}

int main() {
	buffer_begin(buffer);
	fastWriting();
	//fastReading();
	return (EXIT_FAILURE);
}


