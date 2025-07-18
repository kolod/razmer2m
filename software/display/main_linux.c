#ifdef MODE_TEST
#include <stdlib.h>
#include "config.h"
#include "modules/app/interface.h"

int main() {
	app_begin();
	while(1) {
		app_run();
	}
	return EXIT_FAILURE;
}
#endif

