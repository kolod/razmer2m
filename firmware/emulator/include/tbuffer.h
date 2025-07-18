#pragma once

#include <stdint.h>
#include "config.h"

class TBuffer {
private:
	uint8_t items[RAZMER2M_SIGNS_COUNT];
	int error_index;
	
	void goToNextError();
	void setCurrentItemError(bool v);

public:
	int getCount();
	void reset();
	void changeData();
	void changeError();
	uint8_t getItemSign(uint8_t item_index);
	bool getItemError(uint8_t item_index);
	uint8_t getItemCode(uint8_t item_index);
};
