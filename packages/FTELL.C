#include <stdio.h>

long int
ftell(fp)
FILE *fp;
{
long int baseoffset;

if(fp->_seekfunc == NULL)
	baseoffset = 0;
else	{
#ifdef _IOFPTR
	if(fp->_flag & _IOFPTR)
		baseoffset = (*fp->_seekfunc)(fp->_fptr, 0L, 1);
	else
#endif
		baseoffset = (*fp->_seekfunc)(fp->_file, 0L, 1);
	}

if(fp->_base == NULL)
	return baseoffset;

if(fp->_flag & _IOREAD)
	return baseoffset - fp->_cnt;
else	return baseoffset + (fp->_ptr - fp->_base);
}
