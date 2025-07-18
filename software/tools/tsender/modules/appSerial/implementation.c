#include "interface.h"
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include "../connection.h"
#include "../serial/interface.h"
#include "../debug/interface.h"
#include "../esbuffer/interface.h"
#include "../userInput/interface.h"

static int serial_fd;

static void checkWriteResult(ssize_t r, size_t in, int fd){
	if (r < in) {
		if(r == -1){
			if(!serial_alive(fd)){
				putsde("file not found\n");
				return;
			}
			perror("write()");
			return;
		}
		printde("expected to write %zu but %zd done\n", in, r);
		return;
	}
}

static void writeToSerial(const void *buf, size_t sz){
	ssize_t n = write(serial_fd, buf, sz);
	checkWriteResult(n, sz, serial_fd);
	serial_flush(serial_fd);
}

static void sendBuffer(){
	size_t bl = esbuffer_getLength();
	uint8_t buffer[bl];
	for(size_t i=0; i<bl; i++){
		buffer[i] = esbuffer_getItemCode(i);
	}
	size_t sz = bl * sizeof(buffer[0]);
	writeToSerial(buffer, sz);
}

static void sendChar(char v){
	writeToSerial(&v, sizeof v);
}

void appSerial_sendBuffer(){
	//printdo("send: %s\n\n", buf);
	//serial_flush(serial_fd);
	sendChar(CONNECTION_PACKAGE_DELIMITER_START);
	sendBuffer();
	sendChar(CONNECTION_PACKAGE_DELIMITER_STOP);
}

static result_t startDevice(){
	const char *default_path = "/dev/ttyUSB0";
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

static void end_REAL(){
	close(serial_fd);
}

static void end_DUMMY(){
	;
}

static void (*end)() = end_DUMMY;

void appSerial_begin(){
	if(startDevice() == RESULT_SUCCESS){
		end = end_REAL;
	}
}

void appSerial_end(){
	end();
}
