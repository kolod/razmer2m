#include "Arduino.h"
#include "debug.h"
#include "config.h"

void debug_echo() {	
	digitalWrite(DLED_PIN, HIGH);
	delayMicroseconds(100);
	digitalWrite(DLED_PIN, LOW);
}
