#include <stdio.h>
#include "funcs.h"

setvbuf(fp, buf, type, size)
register FILE *fp;
char *buf;
int type;
int size;
{
if((type == _IOFBF || type == _IOLBF) && buf == NULL)
	{
	if(size == 0)
		size = BUFSIZ;		/* fstat? */
#ifdef SETBUFANYTIME
	if(fp->_flag & _IOMYBUF)
		buf = (*_reallocfunc)(fp->_base, size);
	else
#endif
		buf = (*_mallocfunc)(size);

	if(buf == NULL)
		{
		if(fp->_errfunc != NULL)
			(*fp->_errfunc)(fp->_filename, 'm', fp);	/* ? */
		return EOF;
		}

	type |= _IOMYBUF;
	}

/*
 *  #ifdef SETBUFANYTIME (nowhere near finished) still needs to:
 *	copy if going from user buf to _IOMYBUF
 *	flush if buf gets smaller (or _IONBF)
 *	skip following free if reallocating _IOMYBUF
 *	preserve _ptr and _cnt if partially-full buffer
 */

if(fp->_base != NULL && (fp->_flag & _IOMYBUF))
	(*_freefunc)(fp->_base);

fp->_flag &= ~(_IOFBF | _IONBF | _IOLBF | _IOMYBUF);


if(type & _IONBF)
	{
	buf = &fp->_tinybuf;
	size = 1;
	}

fp->_base = fp->_ptr = buf;
fp->_bufsiz = size;

if((fp->_flag & (_IOREAD
#ifdef READWRITE
			 | _IORW
#endif
				)) || (type & (_IONBF
#ifndef PUTCLBUF
						      | _IOLBF
#endif
							      )))
	fp->_cnt = 0;
else	fp->_cnt = size;

fp->_flag |= type;

return 0;
}
