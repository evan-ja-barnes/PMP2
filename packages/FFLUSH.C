#include <stdio.h>

fflush(fp)
register FILE *fp;
{
register int n;
int r = 0;
#ifdef READWRITE

if(fp->_flag & _IORW)
	fp->_flag &= ~(_IOREAD | _IOWRT);

#endif

if((fp->_flag & (_IOWRT | _IORW)) && fp->_base != NULL)
	{
	if(fp->_flsbuf != NULL)		/* NULL means standard _flsbuf */
		{
		if(fp->_ptr > fp->_base)
			return 0;

		fp->_cnt++;
		return (*fp->_flsbuf)(*--fp->_ptr, fp);
		}
	
	if((n = fp->_ptr - fp->_base) > 0)
		{
		r = _fwrite(fp, fp->_base, n);

		if(r < 0)
			r = EOF;

		fp->_ptr = fp->_base;		/* moving these outside of */
		fp->_cnt = fp->_bufsiz;		/* writeability test would */
						/* allow "flushing" input */
						/* fp's, whatever that means */
		}
	}

return r;
}
