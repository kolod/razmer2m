#pragma once

#include <stdint.h>
#include "config.h"
#include "yn_t.h"

class TBuffer {
private:
	uint8_t items[RAZMER2M_SIGNS_COUNT];
	int error_index;
	
	void goToNextError();
	void setCurrentItemError(yn_t v);

public:
	int getCount();
	void reset();
	void changeData();
	void changeError();
	uint8_t getItemSign(uint8_t item_index);
	yn_t getItemError(uint8_t item_index);
	uint8_t getItemCode(uint8_t item_index);
};
