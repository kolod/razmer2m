#include "interface.h"

#include <stdio.h>

char userInput_getFirstChar(){
	char fc = 0;
	for(size_t i=0; ;i++){
		int s = fgetc(stdin);
		if(s==EOF){
			perror("getFirstChar()");
			continue;
		}
		if(s=='\n'){
			break;
		}
		if(i == 0){
			fc = s;
			continue;
		}
	}
	return fc;
}

void userInput_flush(){
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

