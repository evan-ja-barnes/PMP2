#include <stdio.h>

_initsfile(fp)
register FILE *fp;
{
_initfile(fp);

fp->_flag = _IOSTRG;

/* _IOSTRG should keep I/O functions from getting called, but just in case: */

fp->_readfunc = fp->_writefunc = NULL;
fp->_seekfunc = NULL;
fp->_closefunc = NULL;

fp->_file = -1;
}
