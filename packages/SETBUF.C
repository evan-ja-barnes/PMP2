#include <stdio.h>

extern int (*_freefunc)();

setbuf(fp, buf)
FILE *fp;
char *buf;
{
if(fp->_flag & _IOMYBUF)
	(*_freefunc)(fp->_base);

fp->_flag &= ~(_IOMYBUF | _IONBF);

if(buf == NULL)
	{
	fp->_base = fp->_ptr = &fp->_tinybuf;
	fp->_cnt = fp->_bufsiz = 1;
	fp->_flag |= _IONBF;
	}
else	{
	fp->_base = fp->_ptr = buf;
	fp->_bufsiz = BUFSIZ;

	/*
	 *  The same issue arises here as in fseek.
	 *  Don't worry about it much, since nobody ever said
	 *  setbuf was supposed to work after a read or write
	 *  (i.e. when _base non-NULL).
	 */

	if(fp->_flag & _IOWRT)
		fp->_cnt = fp->_bufsiz;
	else	fp->_cnt = 0;
	}
}
