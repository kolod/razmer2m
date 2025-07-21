#include "interface.h"

#ifdef MODE_TEST
#include <stdio.h>
#endif
#include "../viv_t/interface.h"
#include "../tcode/interface.h"
#include "../segIndicator/interface.h"
#include "../segIndicator/interface.h"
#include "../asciiBcd/interface.h"

#define DOT_COLUMN_INDEX			2
#define SIGN_COLUMN_INDEX			6
#define CR_TO_ITEM_INDEX(column_index, row_index) column_index * RAZMER2M_ROWS_COUNT + row_index

static void changeError_SHOW(DBuffer *self);
static void changeError_HIDE(DBuffer *self);

static viv_t checkColumnIndex(int v){
	if(v >= 0 && v < RAZMER2M_COLUMNS_COUNT){
		return VALID;
	}
	return INVALID;
}

static uint8_t codeToSegment(uint8_t v){
	return segIndicator_encodeASCIISwap(bcdToAscii(tcode_getSign(v)));
}

static yn_t updateItem(DBuffer *self, int column, int row, uint8_t code){
	int index = CR_TO_ITEM_INDEX(column, row);
	uint8_t sv = codeToSegment(code);
	if(column == DOT_COLUMN_INDEX) {
		sv = segIndicator_setDot(sv);
	} else {
		if(column == SIGN_COLUMN_INDEX){
			uint8_t sign = tcode_getSign(code);
			if(sign == RAZMER2M_CODE_PLUS){
				sv = segIndicator_encodeASCIISwap('+');
			} else if(sign == RAZMER2M_CODE_MINUS){
				sv = segIndicator_encodeASCIISwap('-');
			} else {
				sv = segIndicator_encodeASCIISwap('?');
			}
		}
	}
	int f = 0;
	if(tcode_getError(code) == NO){
		if(self->segments[index] != sv){
			self->segments[index] = sv;
			f = 1;
		}
	}
	self->codes[index] = code;
	if(f) return YES;
	return NO;
}

static yn_t indexWithDot(int v){
	int offset = DOT_COLUMN_INDEX * RAZMER2M_ROWS_COUNT;
	if(v == offset + 0 || v == offset + 1 || v == offset + 2 || v == offset + 3){
		return YES;
	}
	return NO;
}

static yn_t markErrorSegments(DBuffer *self, uint8_t mark){
	int f = 0;
	for(int i=0; i<RAZMER2M_SIGNS_COUNT; i++){
		if(tcode_getError(self->codes[i]) == YES){
			self->segments[i] = mark;
			f = 1;
		}
	}
	if(f){
		return YES;
	}
	return NO;
}

static void changeError_SHOW(DBuffer *self){
	int f = 0;
	for(int i=0; i<RAZMER2M_SIGNS_COUNT; i++){
		uint8_t code = self->codes[i];
		if(tcode_getError(self->codes[i]) == YES){
			uint8_t v = codeToSegment(code);
			if(indexWithDot(i) == YES){
				v = segIndicator_setDot(v);
			}
			if(v == 0) v = 0xff;
			self->segments[i] =  v;
			f = 1;
		}
	}
	if(f){
		self->fresh = YES;
	}
	self->changeError = changeError_HIDE;
}

static void changeError_HIDE(DBuffer *self){
	if(markErrorSegments(self, 0) == YES){
		self->fresh = YES;
	}
	self->changeError = changeError_SHOW;
}

void dbuffer_begin(DBuffer *self){
	for(int i=0; i<RAZMER2M_SIGNS_COUNT; i++){
		self->codes[i] = 0;
		self->segments[i] = 0;
	}
	self->fresh = YES;
	self->changeError = changeError_SHOW;
}

yn_t dbuffer_hasNewData(DBuffer *self){
	return self->fresh;
}

void dbuffer_changeError(DBuffer *self){
	self->changeError(self);
}


result_t dbuffer_getSegmentFromColumn(DBuffer *self, int index, uint8_t *b1, uint8_t *b2, uint8_t *b3, uint8_t *b4){
	if(checkColumnIndex(index) == VALID){
		*b1 = self->segments[CR_TO_ITEM_INDEX(index, 0)];
		*b2 = self->segments[CR_TO_ITEM_INDEX(index, 1)];
		*b3 = self->segments[CR_TO_ITEM_INDEX(index, 2)];
		*b4 = self->segments[CR_TO_ITEM_INDEX(index, 3)];
		self->fresh = NO;
		return RESULT_SUCCESS;
	}
	return RESULT_FAILURE;
}

result_t dbuffer_setCodeToColumn(DBuffer *self, int index, uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4){
	if(checkColumnIndex(index) == VALID){
		yn_t r1 = updateItem(self, index, 0, b1);
		yn_t r2 = updateItem(self, index, 1, b2);
		yn_t r3 = updateItem(self, index, 2, b3);
		yn_t r4 = updateItem(self, index, 3, b4);
		if(r1 == YES || r2 == YES || r3 == YES || r4 == YES) self->fresh = YES;
		return RESULT_SUCCESS;
	}
	return RESULT_FAILURE;
}

#ifdef MODE_TEST
static void clearScr(){
	printf("\033c");
}

static void printSignsForRow(DBuffer *self, int row){
	for(int column=0; column<RAZMER2M_COLUMNS_COUNT; column++){
		int index = RAZMER2M_SIGNS_COUNT - (column * RAZMER2M_ROWS_COUNT) - (RAZMER2M_ROWS_COUNT - row - 1) - 1;
		printf("%c ", bcdToAscii(tcode_getSign(self->codes[index])));
	}
}

static void printErrorsForRow(DBuffer *self, int row){
	for(int column=0; column<RAZMER2M_COLUMNS_COUNT; column++){
		int index = RAZMER2M_SIGNS_COUNT - (column * RAZMER2M_ROWS_COUNT) - (RAZMER2M_ROWS_COUNT - row - 1) - 1;
		if(tcode_getError(self->codes[index]) == YES){
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

void dbuffer_print(DBuffer *self){
	clearScr();
	printLine();
	for(int row=0; row<RAZMER2M_ROWS_COUNT; row++){
		printf("|"); printSignsForRow(self, row); printf("|\n");
		printf("|"); printErrorsForRow(self, row); printf("|\n");
	}
	printLine();
}
#endif
