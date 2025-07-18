#include "interface.h"

#include "../Arduino.h"
#include "../gpio/interface.h"
#include "../logicLevel_t/interface.h"
#include "../printer/interface.h"

#define VDB_HIGH_LEVEL_STRING		"1"
#define VDB_LOW_LEVEL_STRING		"0"
#define VDB_SINGLE_TIME_US			100

void vdb_reset(VDBuffer *self){
	for(size_t i = 0; i<VDBUFFER_LENGTH; i++){
		VDBufferItem *item = &self->items[i];
		vdbi_reset(item);
	}
	self->count = 0;
}

void vdb_addItem(VDBuffer *self, uint8_t value, unsigned long duration){
	if(self->count < VDBUFFER_LENGTH){
		vdbi_set(&self->items[self->count], value, duration);
		self->count++;
	}
}

yn_t vdb_done(VDBuffer *self){
	if(self->count < VDBUFFER_LENGTH){
		return NO;
	}
	return YES;
}

static void printSignal(VDBuffer *self, const char *description, logicLevel_t (*getLevelFunction)(uint8_t)){
	pr_printStr(description); pr_printStr(": ");
	for(size_t i = 0; i<VDBUFFER_LENGTH; i++){
		VDBufferItem *item = &self->items[i];
		unsigned long duration = vdbi_getDuration(item);
		logicLevel_t level = getLevelFunction(vdbi_getValue(item));
		const char *lc = VDB_HIGH_LEVEL_STRING;
		if(level == LOGIC_LEVEL_LOW){
			lc = VDB_LOW_LEVEL_STRING;
		}
		unsigned int count = duration / VDB_SINGLE_TIME_US;
		for(int j=0; j<count; j++){
			pr_printStr(lc);
		}
	}
	pr_printNl();
}

void vdb_print(VDBuffer *self){
	pr_printStr("Horizontal scale, (us/character): "); pr_printInt(VDB_SINGLE_TIME_US); pr_printNl();
	printSignal(self, "A7", gpio_getA7Level);
	printSignal(self, "B0", gpio_getB0Level);
	printSignal(self, "B1", gpio_getB1Level);
	printSignal(self, "B2", gpio_getB2Level);
	printSignal(self, "B3", gpio_getB3Level);
	printSignal(self, "B4", gpio_getB4Level);
}
