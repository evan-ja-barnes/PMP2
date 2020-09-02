#include <stdio.h>
#include "funcs.h"

_saflsbuf(c, fp)
char c;
register FILE *fp;
{
int chunk = 10;				/* make it global so tunable? */
int newsize;
char *newbase;

newsize = fp->_bufsiz + chunk;

#ifndef SAFEREALLOC
if(fp->_base == NULL)
	newbase = (*_mallocfunc)(newsize);
else
#endif
	newbase = (*_reallocfunc)(fp->_base, newsize);

if(newbase == NULL)
	{
	if(fp->_errfunc != NULL)
		(*fp->_errfunc)(fp->_filename, 'm', fp);	/* ? */
	return EOF;
	}

/* relocate _ptr in case _base moved */

fp->_ptr = newbase + (fp->_ptr - fp->_base);

fp->_base = newbase;
fp->_bufsiz = newsize;

fp->_cnt += chunk - 1;	/* -1 because we're about to put a char */

/* could/should use putc(), but paranoid about looping */

return *fp->_ptr++ = c;		/* mask & 0377? */
}

_initsafile(fp)
register FILE *fp;
{
_initsfile(fp);

fp->_base = fp->_ptr = NULL;
fp->_cnt = fp->_bufsiz = 0;
fp->_flag |= _IOWRT;
fp->_flsbuf = _saflsbuf;
}

char *
_safinish(fp)
register FILE *fp;
{
return (*_reallocfunc)(fp->_base, fp->_bufsiz - fp->_cnt);
}
