/*
 *  Discard any buffering for stream, without doing I/O.
 *  For input streams, equivalent to some implementation's fflush.
 */

#include <stdio.h>

fabort(fp)
register FILE *fp;
{
fp->_ptr = fp->_base;

if(fp->_flag & (_IOREAD | _IONBF
#ifdef READWRITE
				 | _IORW
#endif
#ifndef PUTCLBUF
					 | _IOLBF
#endif
						  ))
	fp->_cnt = 0;
else	fp->_cnt = fp->_bufsiz;
}
