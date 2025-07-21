#include <Arduino.h>

#include "config.h"
#include "modules/app/interface.h"


Buffer buffern;
Buffer *buffer = &buffern;

void setup() {
	delay(START_DELAY_MS);
	gpio_begin();
	serial_begin(CONNECTION_UART_BAUD_RATE, CONNECTION_UART_DPS);
	reader_begin();
	writer_begin();
	buffer_begin(buffer);
}

void loop() {
	reader_run(buffer);
	writer_run(buffer);
}
