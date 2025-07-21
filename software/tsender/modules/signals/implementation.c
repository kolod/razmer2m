//					implementation dependencies
#include "interface.h"
#include <signal.h>

void appSignals_begin(void (*onExitFunction)(int)){
	signal(SIGINT, onExitFunction);
	signal(SIGTERM, onExitFunction);
	signal(SIGHUP, onExitFunction);
	signal(SIGPIPE, onExitFunction);
}
