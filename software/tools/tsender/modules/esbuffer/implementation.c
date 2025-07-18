#include "interface.h"

#include <stdio.h>
#include "../razmer2m1104.h"
#include "../asciiBcd/interface.h"
#include "../tcode/interface.h"
#include "../connection.h"

typedef struct esbufferItem_st{
	uint8_t sign;
	yn_t error;
} EsbufferItem;

static EsbufferItem buffer[RAZMER2M_SIGNS_COUNT];

static int error_index = 0;

void esbuffer_reset(){
	int row = 1;
	int v = 1;
	for(int i=0; i<RAZMER2M_SIGNS_COUNT - 4; i++){
		buffer[i].sign = v;
		if(row < 4){
			row++;
		} else {
			row = 1;
			v++;
		}
		buffer[i].error = NO;
	}
	for(int i=RAZMER2M_SIGNS_COUNT - 4; i<RAZMER2M_SIGNS_COUNT; i++){
		EsbufferItem *item = &buffer[i];
		item->sign = RAZMER2M_CODE_PLUS;
		buffer[i].error = NO;
	}
}

void esbuffer_changeData(){
	for(int i=0; i<RAZMER2M_SIGNS_COUNT - 4; i++){
		EsbufferItem *item = &buffer[i];
		uint8_t v1 = item->sign;
		if(v1 < 9){
			item->sign += 1;
		} else {
			item->sign = 0;
		}
	}
	for(int i=RAZMER2M_SIGNS_COUNT - 4; i<RAZMER2M_SIGNS_COUNT; i++){
		EsbufferItem *item = &buffer[i];
		uint8_t v1 = item->sign;
		if(v1 == RAZMER2M_CODE_PLUS){
			item->sign = RAZMER2M_CODE_MINUS;
		} else {
			item->sign = RAZMER2M_CODE_PLUS;
		}
	}
}

static void goToNextError(){
	if(error_index >= 0 && error_index < RAZMER2M_SIGNS_COUNT){
		error_index++;
	} else {
		error_index = 0;
	}
}

static void setCurrentItemError(yn_t v){
	if(error_index >= 0 && error_index < RAZMER2M_SIGNS_COUNT){
		buffer[error_index].error = v;
	}
}

void esbuffer_changeNextItemError(){
	setCurrentItemError(NO);
	goToNextError();
	setCurrentItemError(YES);
}

uint8_t esbuffer_getItemSign(int item_index){
	if(item_index >= 0 && item_index < RAZMER2M_SIGNS_COUNT){
		return buffer[item_index].sign;
	}
	return 0;
}

yn_t esbuffer_getItemError(int item_index){
	if(item_index >= 0 && item_index < RAZMER2M_SIGNS_COUNT){
		return buffer[item_index].error;
	}
	return NO;
}

uint8_t esbuffer_getItemCode(int item_index){
	uint8_t out = esbuffer_getItemSign(item_index);
	yn_t e = esbuffer_getItemError(item_index);
	if(e == YES){
		tcode_setError(&out);
	}
	return out;
}

result_t esbuffer_setItemByCode(uint8_t code, int item_index){
	if(item_index >= 0 && item_index < RAZMER2M_SIGNS_COUNT){
		EsbufferItem *item = &buffer[item_index];
		item->sign = tcode_getSign(code);
		item->error = tcode_getError(code);
		return RESULT_SUCCESS;
	}
	return RESULT_FAILURE;
}

size_t esbuffer_getLength(){
	return RAZMER2M_SIGNS_COUNT;
}

static int getIndexByRowColumn(int row, int column){
	return RAZMER2M_SIGNS_COUNT - (column * RAZMER2M_ROWS_COUNT) - (RAZMER2M_ROWS_COUNT - row - 1) - 1;
}

static void printSignsForRow(int row){
	int index = getIndexByRowColumn(row, 0);
	uint8_t sign = buffer[index].sign;
	if(sign == RAZMER2M_CODE_PLUS){
		printf("%c ", '+');
	} else if(sign == RAZMER2M_CODE_MINUS){
		printf("%c ", '-');
	} else {
		printf("%c ", '?');
	}
	for(int column=1; column<RAZMER2M_COLUMNS_COUNT; column++){
		int index = getIndexByRowColumn(row, column);
		printf("%c ", bcdToAscii(buffer[index].sign));
	}
}

static void printErrorsForRow(int row){
	for(int column=0; column<RAZMER2M_COLUMNS_COUNT; column++){
		int index = getIndexByRowColumn(row, column);
		if(buffer[index].error == YES){
			printf("^ ");
		} else {
			printf("  ");
		}
	}
}

static void printLine(){
	printf("+");
	for(int column=0; column<RAZMER2M_COLUMNS_COUNT; column++){
		printf("--");
	}
	printf("+\n");
}

void esbuffer_print(){
	printLine();
	for(int row=0; row<RAZMER2M_ROWS_COUNT; row++){
		printf("|"); printSignsForRow(row); printf("|\n");
		printf("|"); printErrorsForRow(row); printf("|\n");
	}
	printLine();
}
