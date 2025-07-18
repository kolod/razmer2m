#include "interface.h"

#include "../TBuffer/interface.h"

TBuffer buffern;
TBuffer *buffer = &buffern;

void esbuffer_reset(){
	tbuffer_reset(buffer);
}

void esbuffer_changeData(){
	tbuffer_changeData(buffer);
}

void esbuffer_changeNextItemError(){
	tbuffer_changeError(buffer);
}

uint8_t esbuffer_getItemSign(int item_index){
	return tbuffer_getItemSign(buffer, item_index);
}

yn_t esbuffer_getItemError(int item_index){
	return tbuffer_getItemError(buffer, item_index);
}
