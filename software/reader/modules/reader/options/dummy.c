#pragma message ( "\n\n \
	Reader: dummy.\n \
" )
#ifdef MODE_TEST
#include <stdio.h>
#endif

void reader_begin(){
#ifdef MODE_TEST
	printf("hellow from dummy reader\n");
#endif
	;
}

void reader_run(Buffer *buffer){
	;
}
