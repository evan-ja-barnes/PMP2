#include <stdio.h>

ungetc(c, fp)
int c;
FILE *fp;
{
if(c == EOF || !(fp->_flag & _IOREAD))
	return EOF;

if(fp->_base == NULL)
	{
	/*
	 *  Could call _getbuf, but the doc sez you've gotta have read
	 *  at least one character, which will have taken care of it.
	 */

	return EOF;
	}

if(fp->_cnt < 0)	/* should never happen, given paranoid _filbuf */
	return EOF;

if(fp->_ptr > fp->_base)
	{
	fp->_ptr--;
	if(*fp->_ptr != c)
		*fp->_ptr = c;
	fp->_cnt++;
	fp->_flag &= ~_IOEOF;
	return c;
	}

/*
 *  This case (_ptr == _base) should only be reached if an ungetc
 *  is attempted immediately after an f*open or fseek, which is
 *  not required to work.
 */

return EOF;
}
