#include <stdio.h>

extern FILE *_lastiob;
extern FILE *_ioblist;

_scanfiles(func)
int (*func)();
{
register FILE *fp;

for(fp = _iob; fp < _lastiob; fp++)
	if(fp->_flag != 0)
		(*func)(fp);

for(fp = _ioblist; fp != NULL; fp = fp->_next)
	(*func)(fp);
}
