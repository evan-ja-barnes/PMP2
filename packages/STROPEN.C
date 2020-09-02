#include <stdio.h>

extern FILE *_getfile();

FILE *
stropen(str, mode)
char *str;
char *mode;
{
register FILE *fp;

if((fp = _getfile()) == NULL)
	return NULL;

_initsfile(fp);

fp->_base = fp->_ptr = str;

if(*mode == 'r')
	{
	fp->_cnt = strlen(str);
	fp->_flag |= _IOREAD;
	}
else if(*mode == 'w' || *mode == 'a')
	{
	fp->_cnt = 32767;		/* oughta be INT_MAX or something */
	fp->_flag |= _IOWRT;
	if(*mode == 'a')
		fp->_ptr += strlen(str);
	}
else	{
	_freefile(fp);
	return NULL;
	}

return fp;
}
