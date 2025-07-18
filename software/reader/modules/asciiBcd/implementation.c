#include "interface.h"

const static char bcdToAsciiMapping[] = {
	'0',
	'1',
	'2',
	'3',
	'4',
	'5',
	'6',
	'7',
	'8',
	'9',
	'*',
	'#',
	'+',
	'-'
};
#define BCD_TO_ASCII_ARR_LENGTH		(sizeof bcdToAsciiMapping / sizeof (bcdToAsciiMapping[0]))

int asciiToBcd(char v){
	for(int i=0; i<BCD_TO_ASCII_ARR_LENGTH; i++){
		if(bcdToAsciiMapping[i] == v){
			return i;
		}
	}
	return 0;
}

char bcdToAscii(int v){
	if(v > 0 && v < BCD_TO_ASCII_ARR_LENGTH){
		return bcdToAsciiMapping[v];
	}
	return 0;
}
