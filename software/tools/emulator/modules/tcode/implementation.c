#include "interface.h"

#include "../connection.h"

uint8_t tcode_getSign(uint8_t code){
	return code & CONNECTION_ERROR_MASK_0;
}

yn_t tcode_getError(uint8_t code){
	if(code & CONNECTION_ERROR_MASK_1){
		return YES;
	}
	return NO;
}

void tcode_setError(uint8_t *code){
	uint8_t v = *code;
	v |= CONNECTION_ERROR_MASK_1;
	*code = v;
}

void tcode_clearError(uint8_t *code){
	uint8_t v = *code;
	v &= CONNECTION_ERROR_MASK_0;
	*code = v;
}


