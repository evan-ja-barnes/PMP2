#include <stdio.h>
#include "funcs.h"

FILE *
freopen(filename, mode, fp)
char *filename;
char *mode;
register FILE *fp;
{
int fd;
int flag;

if(*mode == 'r')
	{
	if((fd = (*_openfunc)(filename, 0)) < 0)
		return NULL;
	flag = _IOREAD;
	}
else	{
	if(*mode == 'w' || (fd = (*_openfunc)(filename, 1) < 0))
		fd = (*_creatfunc)(filename, 0666);

	if(fd < 0)
		return NULL;

	if(*mode == 'a')
		(*_seekfunc)(fd, 0L, 2);

	flag = _IOWRT;
	}

/* gotta effectively fclose, no? */

_initfile(fp);

fp->_file = fd;
fp->_flag |= flag;
if((fp->_filename = (*_mallocfunc)(strlen(filename) + 1)) != NULL)
	strcpy(fp->_filename, filename);

return fp;
}
