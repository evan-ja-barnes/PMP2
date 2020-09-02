#include <stdio.h>
#include "funcs.h"

fsetfuncs(fp, readfunc, writefunc, seekfunc, closefunc)
FILE *fp;
int (*readfunc)(), (*writefunc)();
long int (*seekfunc)();
int (*closefunc)();
{
if(fp == NULL)
	{
	_readfunc = readfunc;
	_writefunc = writefunc;
	_seekfunc = seekfunc;
	_closefunc = closefunc;
	}
else	{
	fp->_readfunc = readfunc;
	fp->_writefunc = writefunc;
	fp->_seekfunc = seekfunc;
	fp->_closefunc = closefunc;
	}
}
