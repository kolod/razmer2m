#include "interface.h"

#include "../gpio/interface.h"
static void switch_ON();
static void switch_OFF();
static void (*switchFun)() = switch_ON;
void debug_echo(){
	gpio_blinkLed();
}
static void switch_ON(){
	gpio_ledOn();
	switchFun = switch_OFF;
}
static void switch_OFF(){
	gpio_ledOff();
	switchFun = switch_ON;
}
void debug_switch(){
	gpio_blinkLed();
}
