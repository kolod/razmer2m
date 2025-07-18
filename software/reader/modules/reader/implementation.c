#include "interface.h"

#ifdef MODE_TEST
#include "../../config.h"
#endif

#ifdef READER_REAL
#include "options/real.c"
#else
#ifdef READER_IMITATION
#include "options/imitation.c"
#else
#include "options/dummy.c"
#endif
#endif
