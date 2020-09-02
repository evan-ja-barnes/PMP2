#include <stdio.h>

extern FILE *_getfile();

FILE *
strnopen(str, n, mode)
char *str;
int n;
char *mode;
{
register FILE *fp;

if((fp = _getfile()) == NULL)
	return NULL;

_initfile(fp);

fp->_base = fp->_ptr = str;
fp->_cnt = n;

if(*mode == 'r')
	{
	fp->_flag |= _IOREAD;
	}
else if(*mode == 'w' || *mode == 'a')
	{
	fp->_flag |= _IOWRT;
	if(*mode == 'a')
		{
		int len;

		/*
		 *  Ambiguity: n is size of buffer, not length
		 *  of existing string.
		 */

		len = strlen(str);
		if(len > n)
			len = n;

		fp->_ptr += len;
		fp->_cnt -= len;
		}
	}
else	{
	_freefile(fp);
	return NULL;
	}

return fp;
}
