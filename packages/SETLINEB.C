#include <stdio.h>

setlinebuf(fp)
register FILE *fp;
{
if(fp->_base == NULL)
	setvbuf(fp, (char *)NULL, _IOLBF, BUFSIZ);
else if(!(fp->_flag & _IONBF))
	fp->_flag |= _IOLBF;
}
