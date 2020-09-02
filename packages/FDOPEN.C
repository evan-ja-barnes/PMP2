#include <stdio.h>

extern FILE *_getfile();
extern long int lseek();

FILE *
fdopen(fd, mode)
int fd;
char *mode;
{
register FILE *fp;

if((fp = _getfile()) != NULL)
	{
	fp->_file = fd;
	if(*mode == 'r')
		fp->_flag |= _IOREAD;
	else	{
		fp->_flag |= _IOWRT;
		if(*mode == 'a')
			lseek(fd, 0L, 2);	/* should it do this? */
		}
	}

return fp;
}
