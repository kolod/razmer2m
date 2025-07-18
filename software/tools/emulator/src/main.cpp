#include <Arduino.h>
#include "config.h"
#include "writer.h"

void setup() {
	writer_begin();
}

void loop() {
	writer_run();
}
