#include "interface.h"
#include "../stime_t/interface.h"

#pragma message ( "\n\n \
	Arduino: for linux.\n \
" )

void pinMode(uint8_t pin, uint8_t mode){;}

void digitalWrite(uint8_t pin, uint8_t val){;}
int digitalRead(uint8_t pin){return LOW;}
int analogRead(uint8_t pin){return 0;}
void analogReference(uint8_t mode){;}
void analogWrite(uint8_t pin, int val){;}

unsigned long millis(void){
	return stimeToMs(getCurrentTime());
}

unsigned long micros(void){
	return stimeToUs(getCurrentTime());
}

void delay(unsigned long ms){
	stime_sleep(msToStime(ms));
}

void delayMicroseconds(unsigned int us){
	stime_sleep(usToStime(us));
}
