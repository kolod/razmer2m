#pragma message ( "\n\n \
	Printer: Arduino\n \
" )

#include "../../Arduino.h"
#include "../../../config.h"

void pr_begin(){
	Serial.begin(SERIAL_BAUD_RATE);
}

void pr_printStr(const char *v){
	Serial.print(v);
}

void pr_printInt(int v){
	Serial.print(v);
}

void pr_printULong(unsigned long v){
	Serial.print(v);
}

void pr_printNl(){
	Serial.print("\n");
}
