#include "interface.h"
#include "../Arduino.h"
#include "../writer/interface.h"
#include "../../config.h"

void app_begin() {
#ifndef MODE_TEST
	delay(START_DELAY_MS);
#endif
	writer_begin();
}

void app_run(){
	writer_run();
}
