/* called by _filbuf and _flsbuf to get initial buffer */

#include <stdio.h>
#ifdef bsd4_2
#include <sys/types.h>
#include <sys/stat.h>
#endif
#include "funcs.h"

_getbuf(fp)
register FILE *fp;
{
int bufsiz;
#ifdef bsd4_2
struct stat stbuf;
#endif

if(fp->_base != NULL)
	return;

if(!(fp->_flag & _IONBF))
	{
	bufsiz = BUFSIZ;

	if(fp->_readfunc == _readfunc || fp->_writefunc == _writefunc)
		{
		if(fp == stdout && ISATTY(fileno(stdout)))
			fp->_flag |= _IOLBF;

#ifdef bsd4_2
		if(FSTAT(fp->_file, &stbuf) >= 0 && stbuf.st_blksize != 0)
			bufsiz = stbuf.st_blksize;
#endif
		}

	if((fp->_base = (*_mallocfunc)(bufsiz)) != NULL)
		{
		fp->_flag |= _IOMYBUF;
		}
	else	{
		fp->_flag &= ~_IOLBF;
		fp->_flag |= _IONBF;
		}
	}

fp->_cnt = fp->_bufsiz = bufsiz;

if(fp->_flag & _IONBF)
	{
	fp->_base = &fp->_tinybuf;
	fp->_bufsiz = 1;
	fp->_cnt = 0;		/* all putc's call _flsbuf */
	}

fp->_ptr = fp->_base;

#ifndef PUTCLBUF

if(fp->_flag & _IOLBF)
	fp->_cnt = 0;		/* all putc's call _flsbuf */

#endif
}
