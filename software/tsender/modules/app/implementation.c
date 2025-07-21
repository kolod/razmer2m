#include "interface.h"

#include <stdlib.h>
#include "../signals/interface.h"
#include "../writer/interface.h"

static void onExitCommand(){
	writer_end();
	exit(EXIT_SUCCESS);
}

void app_begin(){
	appSignals_begin(onExitCommand);
	writer_begin();
}

void app_run(){
	writer_run();
}
