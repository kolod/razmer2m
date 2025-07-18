#pragma message ( "\n\n \
	Printer: stdio\n \
" )

#include <stdio.h>

void pr_begin(){
	;
}

void pr_printStr(const char *v){
	printf("%s", v);
}

void pr_printInt(int v){
	printf("%d", v);
}

void pr_printULong(unsigned long v){
	printf("%lu", v);
}

void pr_printNl(){
	printf("\n");
}
