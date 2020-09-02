#include <stdio.h>
#include "funcs.h"

extern int READFUNC();
extern int WRITEFUNC();
extern long SEEKFUNC();
extern int CLOSEFUNC();
extern int _errfunc();

FILE _iob[_NFILE] =	/* _NFILE just for compatibility */
	{
	{NULL, 0, NULL, 0, _IOREAD | _IOFBF | _IOSTFN, 0, NULL, 0,
		"standard input", NULL, NULL,
		 READFUNC, WRITEFUNC, SEEKFUNC, CLOSEFUNC, _errfunc, NULL},
	{NULL, 0, NULL, 0, _IOWRT | _IOFBF | _IOSTFN, 1, NULL, 0,
		"standard output", NULL, NULL,
		READFUNC, WRITEFUNC, SEEKFUNC, CLOSEFUNC, _errfunc, NULL},
	{NULL, 0, NULL, 0, _IOWRT | _IONBF | _IOSTFN, 2, NULL, 0,
		"standard error", NULL, NULL,
		READFUNC, WRITEFUNC, SEEKFUNC, CLOSEFUNC, _errfunc, NULL}
	};

FILE *_lastiob = &_iob[sizeof(_iob) / sizeof(*_iob)];
