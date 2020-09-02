#include <stdio.h>

fseek(fp, offset, whence)
FILE *fp;
long int offset;
int whence;
{
fp->_flag &= ~_IOEOF;

/*
 *  A questionable optimization: simply relocate _ptr in buffer, if possible.
 *  Problems are with ungetc:
 *	1. if _ptr ends up smack dab at _base, there's no place to ungetc
 *	2. an ungetc'd character could possibly (?) be written back out
 *	   to the file (horrifying thought)
 *	3. fseek is supposed to undo the effects of an ungetc
 */

if(whence == SEEK_CUR && fp->_base != NULL && (fp->_flag & _IOREAD))
	{
	if((offset > 0 && fp->_cnt - offset >= 0) ||
			fp->_ptr - fp->_base >= -offset)
		{
		fp->_ptr += offset;
		fp->_cnt -= offset;
#ifdef READWRITE
		fp->_flag &= ~_IOWRT;
#endif
		return 0;
		}
	}

if(fp->_flag & _IOWRT)
	fflush(fp);

if(fp->_seekfunc == NULL)
	return EOF;

if(whence == SEEK_CUR && fp->_base != NULL)
	{
	if(fp->_flag & _IOREAD)
		offset -= fp->_cnt;
	else if(fp->_flag & _IOWRT)
		offset += fp->_ptr - fp->_base;
	}

#ifdef READWRITE

if(fp->_flag & _IORW)
	fp->_flag &= ~(_IOREAD | _IOWRT);

#endif

#ifdef _IOFPTR
if(fp->_flag & _IOFPTR)
	(*fp->_seekfunc)(fp->_fptr, offset, whence);
else
#endif
	(*fp->_seekfunc)(fp->_file, offset, whence);

#ifdef notyet

if(r == -1)
	{
	fp->_flag |= _IOERR;
	if(fp->_errfunc != NULL)
		(*fp->_errfunc)(fp->_filename, 's', fp);
	}

#endif

if(fp->_base == NULL)
	return 0;

fp->_ptr = fp->_base;	/* worry that there's now no room for ungetc */

/*
 *  Is this the right way to do this?  Or should it always be set
 *  to 0?  A putc in _IORW mode will cause a flush, even though
 *  there oughta be room.  (_flsbuf will notice _ptr == _base,
 *  though, and not write anything.)  Of course, in _IORW mode,
 *  we don't know whether a putc or getc will happen next.
 */

if(fp->_flag & _IOWRT)
	fp->_cnt = fp->_bufsiz;
else	fp->_cnt = 0;

return 0;
}
