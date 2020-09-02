#include "funcs.h"

/* in a separate file so only loaded if needed */

extern char *REALLOC();

char *(*_reallocfunc)() = REALLOC;
