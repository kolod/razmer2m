#include <stdlib.h>
#include "modules/app/interface.h"

int main() {
	app_begin();
	while(1) {
		app_run();
	}
	return EXIT_FAILURE;
}


