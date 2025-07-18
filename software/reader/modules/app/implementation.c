#include "interface.h"
#ifdef MODE_TEST
#include <stdlib.h>
#include "../signals/interface.h"
#endif
#include "../Arduino.h"
#include "../reader/interface.h"
#include "../writer/interface.h"
#include "../gpio/interface.h"
#include "../serial/interface.h"
#include "../connection.h"
#include "../../config.h"

Buffer buffern;
Buffer *buffer = &buffern;

#ifdef MODE_TEST
static void onExitCommand(){
	exit(EXIT_SUCCESS);
}

void app_begin(){
	appSignals_begin(onExitCommand);
	gpio_begin();
	serial_begin(CONNECTION_UART_BAUD_RATE, CONNECTION_UART_DPS);
	reader_begin();
	writer_begin();
	buffer_begin(buffer);
}
#else
void app_begin() {
	delay(START_DELAY_MS);
	gpio_begin();
	serial_begin(CONNECTION_UART_BAUD_RATE, CONNECTION_UART_DPS);
	reader_begin();
	writer_begin();
	buffer_begin(buffer);
}
#endif

void app_run(){
	reader_run(buffer);
	writer_run(buffer);
}
