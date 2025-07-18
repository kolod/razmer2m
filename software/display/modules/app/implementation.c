#include "interface.h"
#include "../Arduino.h"
#include "../display/interface.h"
#include "../server/interface.h"
#include "../../config.h"

Buffer buffern;
Buffer *buffer = &buffern;

void app_begin() {
#ifndef MODE_TEST
	delay(START_DELAY_MS);
#endif
	buffer_begin(buffer);
	display_begin(COMMUNICATION_TIMEOUT_MS);
	server_begin();
}

void app_run(){
	server_run(buffer);
	display_run(buffer);
}
