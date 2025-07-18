#include "interface.h"

#include <stdlib.h>
#include "../signals/interface.h"
#include "../reader/interface.h"

static void onExitCommand(){
	reader_end();
	exit(EXIT_SUCCESS);
}

void app_begin(){
	appSignals_begin(onExitCommand);
	reader_begin();
}

void app_run(){
	reader_run();
}
