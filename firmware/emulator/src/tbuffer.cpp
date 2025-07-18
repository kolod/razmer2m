#include "tbuffer.h"
#include "ascii2bcd.h"
#include "tcode.h"

int TBuffer::getCount() {
	return RAZMER2M_SIGNS_COUNT;
}

void TBuffer::reset() {
	int row = 1;
	int v = 1;
	for (int i=0; i<RAZMER2M_SIGNS_COUNT - 4; i++) {
		items[i] = v;
		if (row < 4) {
			row++;
		} else {
			row = 1;
			v++;
		}
	}
	for (int i=RAZMER2M_SIGNS_COUNT - 4; i<RAZMER2M_SIGNS_COUNT; i++) {
		items[i] = asciiToBcd('+');
	}
	error_index = 0;
}

void TBuffer::changeData() {
	for (int i=0; i<RAZMER2M_SIGNS_COUNT - 4; i++) {
		uint8_t v0 = items[i];
		uint8_t v1 = tcode_getSign(v0);
		if (v1 < 9) {
			v1 += 1;
		} else {
			v1 = 0;
		}
		if (tcode_getError(v0) == true) {
			tcode_setError(&v1);
		}
		items[i] = v1;
	}

	for (int i=RAZMER2M_SIGNS_COUNT - 4; i<RAZMER2M_SIGNS_COUNT; i++) {
		uint8_t v0 = items[i];
		uint8_t v1 = tcode_getSign(v0);
		uint8_t v2 = asciiToBcd('+');

		if (v1 == asciiToBcd('+')) {
			v2 = asciiToBcd('-');
		}

		if (tcode_getError(v0) == true) {
			tcode_setError(&v2);
		}

		items[i] = v2;
	}
}

void TBuffer::goToNextError() {
	int ei = error_index;
	if (ei >= 0 && ei < RAZMER2M_SIGNS_COUNT) {
		ei++;
	} else {
		ei = 0;
	}
	error_index = ei;
}

void TBuffer::setCurrentItemError(bool v) {
	int ei = error_index;
	if (ei >= 0 && ei < RAZMER2M_SIGNS_COUNT) {
		uint8_t vb = items[ei];
		if (v == true) {
			tcode_setError(&vb);
		} else {
			tcode_clearError(&vb);
		}
		items[ei] = vb;
	}
}

void TBuffer::changeError() {
	setCurrentItemError(false);
	goToNextError();
	setCurrentItemError(true);
}

uint8_t TBuffer::getItemSign(uint8_t item_index) {
	// Check if item_index is valid and return the sign for the item
	if (item_index >= RAZMER2M_SIGNS_COUNT) return 0;
	
	// Get the sign from the tcode
	return tcode_getSign(items[item_index]);
}

bool TBuffer::getItemError(uint8_t item_index) {
	// Check if item_index is valid 
	// and return the sign for the item
	if (item_index >= RAZMER2M_SIGNS_COUNT) return false;

	// Get the error status from the tcode
	return tcode_getError(items[item_index]);
}

uint8_t TBuffer::getItemCode(uint8_t item_index) {
	// Check if item_index is valid and return the sign for the item
	if (item_index >= RAZMER2M_SIGNS_COUNT) return 0;

	// Return the item code directly
	return items[item_index];
}
