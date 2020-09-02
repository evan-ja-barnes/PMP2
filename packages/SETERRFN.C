#include <stdio.h>

extern int (*_deflterrfunc)();

fseterrfunc(fp, errfunc)
FILE *fp;
int (*errfunc)();
{
if(fp == NULL)
	_deflterrfunc = errfunc;
else	fp->_errfunc = errfunc;
}
