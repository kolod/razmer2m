#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>

#include "../../LLBuffer/interface.h"


#include "../../yn_t/implementation.c"
#include "../../LLBufferItem/implementation.c"
#include "../../LLBuffer/implementation.c"

LLBuffer buffern;
LLBuffer *buffer = &buffern;

void fill(){
	while(1){
		llb_addHighTime(buffer, 12);
		llb_addLowTime(buffer, 200);
		if(llb_done(buffer) == YES){
			break;
		}
		llb_addHighTime(buffer, 15);
		llb_addLowTime(buffer, 220);
		if(llb_done(buffer) == YES){
			break;
		}
	}
}

void printResult(){
	unsigned long thmin = llb_getHighTimeMin(buffer);
	unsigned long thmax= llb_getHighTimeMax(buffer);
	unsigned long tlmin = llb_getLowTimeMin(buffer);
	unsigned long tlmax = llb_getLowTimeMax(buffer);
	printf("HIGH: %ld...%ld, LOW: %ld...%ld\n", thmin, thmax, tlmin, tlmax);
}

int main() {
	llb_reset(buffer);
	fill();
	printResult();
	return (EXIT_FAILURE);
}


