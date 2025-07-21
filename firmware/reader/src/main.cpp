#include <Arduino.h>

#include "config.h"
#include "reader.h"
#include "writer.h"
#include "gpio.h"


Buffer buffern;
Buffer *buffer = &buffern;

void setup() {
	delay(START_DELAY_MS);
	gpio_begin();
	Serial.begin(CONNECTION_UART_BAUD_RATE, CONNECTION_UART_DPS);
	reader.begin();
	writer.begin();
	buffer_begin(buffer);
}

void loop() {
	reader.run(buffer);
	writer.run(buffer);
}
