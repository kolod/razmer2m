#include "interface.h"
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include "../connection.h"
#include "../stime_t/interface.h"
#include "../serial/interface.h"
#include "../debug/interface.h"
#include "../esbuffer/interface.h"
#include "../userInput/interface.h"

static int serial_fd;
static appSerialError_t error = APP_SERIAL_ERROR_UNKNOWN;

static void end_REAL();
static void end_DUMMY();
static void (*end)() = end_DUMMY;

static bsfState_t run_DUMMY();
static bsfState_t run_READ();
static bsfState_t run_START_DEVICE();
static bsfState_t (*run)() = run_DUMMY;

static result_t startDevice(){
	const char *default_path = "/dev/ttyUSB1";
	int len = 32;
	char user_path[len];
	memset(user_path, 0, sizeof user_path);
	const char *path = "";
	int baud_rate = CONNECTION_UART_BAUD_RATE;
	while(1){
		while(1){
			printf("type 'd' to start serial (%d %s) or 'o' for more options (d | o):", baud_rate, default_path);
			char s = userInput_getFirstChar();
			if(s == 'd'){
				path = default_path;
				break;
			} else if (s == 'o'){
				while(1){
					printf("baudRate path:");
					int r = fscanf(stdin, "%d %31s", &baud_rate, user_path);
					userInput_flush();
					if(r == 2){
						path = user_path;
						break;
					}
				}
				break;
			} else {
				continue;
			}
		}
		//printf("your path:");
		//if(fscanf(stdin, "%s", user_path) == 1){
			//path = user_path;
		//}
		printf("trying to open: %s ... ", path);
		if(serial_begin(&serial_fd, path, baud_rate, CONNECTION_UART_DPS) != RESULT_SUCCESS){
			printf("failed.\n");
			continue;
		}
		printf("success.\n");
		return RESULT_SUCCESS;
	}
	return RESULT_FAILURE;
}

static bsfState_t run_DUMMY(){
	stime_sleep(usToStime(100));
	return FAILURE;
}

static bsfState_t run_READ(){
	size_t extra_count = 2;
	size_t bl = esbuffer_getLength() + extra_count;
	uint8_t buffer[bl];
	serialError_t r = serial_readResponse(serial_fd, buffer, bl, CONNECTION_PACKAGE_DELIMITER_START, CONNECTION_PACKAGE_DELIMITER_STOP, 1000);
	if(r == SERIAL_ERROR_DEVICE_DISCONNECTED){
		close(serial_fd);
		end = end_DUMMY;
		run = run_START_DEVICE;
		putsde("appSerial: device disconnected\n");
		error = APP_SERIAL_ERROR_DEVICE_DISCONNECTED;
		return FAILURE;
	}
	if(r != SERIAL_ERROR_NONE){
		putsde("appSerial: no data\n");
		error = APP_SERIAL_ERROR_NO_DATA;
		return FAILURE;
	}
	for(size_t i = 1; i < bl - 1; i++){
		result_t r = esbuffer_setItemByCode(buffer[i], i-1);
		if(r != RESULT_SUCCESS){
			putsde("appSerial: failed to add item to buffer\n");
			error = APP_SERIAL_ERROR_INVALID_DATA;
			return FAILURE;
		}
	}
	error = APP_SERIAL_ERROR_NONE;
	return SUCCESS;
}

static bsfState_t run_START_DEVICE(){
	if(startDevice() == RESULT_SUCCESS){
		end = end_REAL;
		run = run_READ;
		error = APP_SERIAL_ERROR_NONE;
		return BUSY;
	}
	return BUSY;
}

appSerialError_t appSerial_getError(){
	return error;
}


static void end_REAL(){
	close(serial_fd);
}

static void end_DUMMY(){
	;
}

void appSerial_begin(){
	run = run_START_DEVICE;
}

bsfState_t appSerial_readInBuffer(){
	return run();
}

void appSerial_end(){
	end();
}
