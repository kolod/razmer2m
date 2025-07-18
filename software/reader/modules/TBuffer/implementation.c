#include "interface.h"
#include "../asciiBcd/interface.h"
#include "../tcode/interface.h"
#include "../viv_t/interface.h"
#include "../razmer2m1104.h"

int tbuffer_getCount(TBuffer *self){
	return RAZMER2M_SIGNS_COUNT;
}

void tbuffer_reset(TBuffer *self){
	int row = 1;
	int v = 1;
	for(int i=0; i<RAZMER2M_SIGNS_COUNT - 4; i++){
		self->items[i] = v;
		if(row < 4){
			row++;
		} else {
			row = 1;
			v++;
		}
	}
	for(int i=RAZMER2M_SIGNS_COUNT - 4; i<RAZMER2M_SIGNS_COUNT; i++){
		self->items[i] = RAZMER2M_CODE_PLUS;
	}
	self->error_index = 0;
}

void tbuffer_changeData(TBuffer *self){
	for(int i=0; i<RAZMER2M_SIGNS_COUNT - 4; i++){
		uint8_t v0 = self->items[i];
		uint8_t v1 = tcode_getSign(v0);
		if(v1 < 9){
			v1 += 1;
		} else {
			v1 = 0;
		}
		if(tcode_getError(v0) == YES){
			tcode_setError(&v1);
		}
		self->items[i] = v1;
	}
	for(int i=RAZMER2M_SIGNS_COUNT - 4; i<RAZMER2M_SIGNS_COUNT; i++){
		uint8_t v0 = self->items[i];
		uint8_t v1 = tcode_getSign(v0);
		uint8_t v2 = RAZMER2M_CODE_PLUS;
		if(v1 == RAZMER2M_CODE_PLUS){
			v2 = RAZMER2M_CODE_MINUS;
		}
		if(tcode_getError(v0) == YES){
			tcode_setError(&v2);
		}
		self->items[i] = v2;
	}
}

static void goToNextError(TBuffer *self){
	int ei = self->error_index;
	if(ei >= 0 && ei < RAZMER2M_SIGNS_COUNT){
		ei++;
	} else {
		ei = 0;
	}
	self->error_index = ei;
}

static void setCurrentItemError(TBuffer *self, yn_t v){
	int ei = self->error_index;
	if(ei >= 0 && ei < RAZMER2M_SIGNS_COUNT){
		uint8_t vb = self->items[ei];
		if(v == YES){
			tcode_setError(&vb);
		} else {
			tcode_clearError(&vb);
		}
		self->items[ei] = vb;
	}
}

void tbuffer_changeError(TBuffer *self){
	setCurrentItemError(self, NO);
	goToNextError(self);
	setCurrentItemError(self, YES);
}

static viv_t checkItemIndex(int v){
	if(v >= 0 && v < RAZMER2M_SIGNS_COUNT){
		return VALID;
	}
	return INVALID;
}

uint8_t tbuffer_getItemSign(TBuffer *self, int item_index){
	if(checkItemIndex(item_index) == VALID){
		return tcode_getSign(self->items[item_index]);
	}
	return 0;
}

yn_t tbuffer_getItemError(TBuffer *self, int item_index){
	if(checkItemIndex(item_index) == VALID){
		return tcode_getError(self->items[item_index]);
	}
	return NO;
}

uint8_t tbuffer_getItemCode(TBuffer *self, int item_index){
	if(checkItemIndex(item_index) == VALID){
		return self->items[item_index];
	}
	return 0;
}

