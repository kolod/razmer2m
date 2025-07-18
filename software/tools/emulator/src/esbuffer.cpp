#include "esbuffer.h"
#include "tbuffer.h"

TBuffer buffer;

void esbuffer_reset(){
	buffer.reset();
}

void esbuffer_changeData(){
	buffer.changeData();
}

void esbuffer_changeNextItemError(){
	buffer.changeError();
}

uint8_t esbuffer_getItemSign(int item_index){
	return buffer.getItemSign(item_index);
}

yn_t esbuffer_getItemError(int item_index){
	return buffer.getItemError(item_index);
}
