#include <stdio.h>

#define TRUE 1
#define FALSE 0

_flsbuf(c, fp)
char c;
register FILE *fp;
{
register int r;
int nwrt;
int bufstyle;
int printedc = FALSE;

if(fp == NULL)		/* would also be nice to check validity somehow */
	{
	/* set errno? */
	/* abort? */

	return EOF;
	}

if(fp->_flsbuf != NULL)
	return (*fp->_flsbuf)(c, fp);

#ifdef READWRITE

if((fp->_flag & _IORW) && (fp->_flag & (_IOREAD | _IOWRT)) == 0)
	fp->_flag |= _IOWRT;

#endif

if(!(fp->_flag & _IOWRT) || (fp->_flag & _IOSTRG) || fp->_writefunc == NULL)
	{
	/* set errno? */
	/* set _IOERR? */
	/* abort? */

	return EOF;
	}

if(fp->_base == NULL)
	_getbuf(fp);

nwrt = fp->_ptr - fp->_base;

bufstyle = fp->_flag & (_IOFBF | _IONBF | _IOLBF);

if(bufstyle == _IOLBF)
	{
#ifdef PUTCLBUF
	if(fp->_cnt > 0)
#else
	if(fp->_ptr < fp->_base + fp->_bufsiz)
#endif
		{
		*fp->_ptr++ = c;
#ifdef PUTCLBUF				/* shouldn't happen */
		fp->_cnt--;
#endif
		nwrt++;
		printedc = TRUE;
		}

#ifndef PUTCLBUF
	if(c != '\n')
		return c;
#endif	
	}

if(nwrt > 0)
	r = _fwrite(fp, fp->_base, nwrt);
else	r = 0;

if(!printedc && (bufstyle == _IONBF || bufstyle == _IOLBF && c == '\n'))
	{
	char realc = c;		/* so can take address portably */
				/* (strictly speaking, declaring formal */
				/*	char c; */
				/* should suffice) */

	if(_fwrite(fp, &realc, 1) < 0)
		r = -1;

	printedc = TRUE;
	}

fp->_ptr = fp->_base;

fp->_cnt = (fp->_flag & (_IONBF
#ifndef PUTCLBUF
				| _IOLBF
#endif
					)) ? 0 : fp->_bufsiz;

if(!printedc)
	{
	*fp->_ptr++ = c;
	fp->_cnt--;
	}

if(r < 0)
	return EOF;

return c & 0377;
}

_fwrite(fp, p, n)
FILE *fp;
char *p;
int n;
{
int r;

if(fp->_writefunc == NULL)
	return -1;

#ifdef _IOFPTR
if(fp->_flag & _IOFPTR)
	r = (*fp->_writefunc)(fp->_fptr, p, n);
else
#endif
	r = (*fp->_writefunc)(fp->_file, p, n);

if(r < 0)		/* if r != n ?? */
	{
	fp->_flag |= _IOERR;
	if(fp->_errfunc != NULL)
		(*fp->_errfunc)(fp->_filename, 'w', fp);
	}

return r;
}

#ifdef FORCECLEANUP

extern int _cleanup();

static int (*dummy)() = _cleanup;

#endif
