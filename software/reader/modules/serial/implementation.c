#include "interface.h"

#include "../Arduino.h"

#ifdef MODE_TEST
#include <stdio.h>
#include "../connection.h"
#include "../tcode/interface.h"
void serial_begin(int baud_rate, int config){
	;
}

static char byteToChar(uint8_t vr){
	uint8_t v = tcode_getSign(vr);
	if(v >= 0 && v <=9){
		return '0' + v;
	}
	if(v == CONNECTION_PACKAGE_DELIMITER_START || v == CONNECTION_PACKAGE_DELIMITER_STOP){
		return v;
	}
	return '?';
}

result_t serial_writeByte(void *v){
	uint8_t *vb = v;
	char pc = byteToChar(*vb);
	printf("%c", pc);
	return RESULT_SUCCESS;
}
#else
void serial_begin(int baud_rate, int config){
	Serial.begin(CONNECTION_UART_BAUD_RATE, CONNECTION_UART_DPS);
}

result_t serial_writeByte(void *v){
	if(Serial.availableForWrite() < 1) {
		return RESULT_FAILURE;
	}
	Serial.write((char *)v, 1);
	return RESULT_SUCCESS;
}
#endif
