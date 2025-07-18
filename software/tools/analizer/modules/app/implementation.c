#include "interface.h"
#include "../Arduino.h"
#include "../gpio/interface.h"
#include "../analizer/interface.h"
#include "../printer/interface.h"
#include "../../config.h"

#ifdef MODE_TEST
#include <stdlib.h>
#include "../signals/interface.h"

static void onExitCommand(){
	exit(EXIT_SUCCESS);
}
#endif

void app_begin() {
#ifndef MODE_TEST
	delay(START_DELAY_MS);
#else
	appSignals_begin(onExitCommand);
#endif
	gpio_begin();
	pr_begin();
	anl_begin();
}

void app_run(){
	anl_run();
}
