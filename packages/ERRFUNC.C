#include <stdio.h>

int _errfunc();

int (*_deflterrfunc)() = _errfunc;

_errfunc(filename, rw, fp)
char *filename;
int rw;
FILE *fp;
{
if(fp == stderr && rw == 'w')	/* avoid infinite loops */
	return;		/* abort? */

/* avoids use of fprintf, so won't force _doprnt */

if(filename != NULL)
	{
	fputs(filename, stderr);
	fputs(": ", stderr);
	}

switch(rw)
	{
	case 'c':
		fputs("close ", stderr);
		break;

	case 'm':
		fputs("memory allocation ", stderr);
		break;

	case 'r':
		fputs("read ", stderr);
		break;

	case 's':
		fputs("seek ", stderr);
		break;

	case 'w':
		fputs("write ", stderr);
		break;

	default:
		fputc(rw, stderr);
	}
fputs(" error: ", stderr);
fflush(stderr);
perror("");

#ifndef WRITEERRNOTFATAL

if(rw == 'w')
	{
	if(fp != NULL)
		_freefile(fp);	/* so it won't get found by _cleanup */
	exit(1);
	}

#endif

/* return value? */
}
