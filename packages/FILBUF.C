#include <stdio.h>

_filbuf(fp)
register FILE *fp;
{
register int r;
char c;

if(fp == NULL)		/* would also be nice to check validity somehow */
	{
	/* set errno? */
	/* call error handler? */
	/* abort? */

	return EOF;
	}

if(fp->_filbuf != NULL)
	return (*fp->_filbuf)(fp);

fp->_cnt = 0;	/* avoid runaway count if repeated getc's at EOF */
		/* also ensures ungetc at EOF works */

#ifdef READWRITE

if((fp->_flag & _IORW) && (fp->_flag & (_IOREAD | _IOWRT)) == 0)
	fp->_flag |= _IOREAD;

#endif

if(!(fp->_flag & _IOREAD) || (fp->_flag & _IOSTRG))
	{
	/* set errno? */
	/* set _IOERR? */
	/* call error handler? */
	/* abort? */

	return EOF;
	}

#ifdef STICKYEOF

if(fp->_flag & _IOEOF)
	return EOF;

#endif

#ifdef STDINFLUSH

if(fp == stdin && (stdout->_flag & _IOLBF))	/* ? */
	fflush(stdout);

#endif

if(fp->_base == NULL)
	_getbuf(fp);

if(fp->_readfunc == NULL)
	r = 0;
else	{
	r = _fread(fp, fp->_base, fp->_bufsiz);

	if(r > 0)
		{
		fp->_cnt = r - 1;
		fp->_ptr = fp->_base;
		c = *fp->_ptr++;
		}
	}

if(r <= 0)
	return EOF;

return c & 0377;
}

_fread(fp, p, n)
register FILE *fp;
char *p;
int n;
{
int r;

#ifdef _IOFPTR
if(fp->_flag & _IOFPTR)
	r = (*fp->_readfunc)(fp->_fptr, p, n);
else
#endif
	r = (*fp->_readfunc)(fp->_file, p, n);

if(r <= 0)
	{
	if(r == 0)
		{
		fp->_flag |= _IOEOF;

#ifdef READWRITE

		if(fp->_flag & _IORW)
			fp->_flag &= ~(_IOREAD | _IOWRT);
#endif
		}
	else	{
		fp->_flag |= _IOERR;
		if(fp->_errfunc != NULL)
			(*fp->_errfunc)(fp->_filename, 'r', fp);
		}
	}

return r;
}
