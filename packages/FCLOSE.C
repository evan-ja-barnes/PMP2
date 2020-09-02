#include <stdio.h>

fclose(fp)
register FILE *fp;
{
int r1, r2;

r1 = fflush(fp);

if(fp->_closefunc != NULL)
	{
#ifdef _IOFPTR
	if(fp->_flag & _IOFPTR)
		r2 = (*fp->_closefunc)(fp->_fptr);
	else
#endif
		r2 = (*fp->_closefunc)(fp->_file);

	if(r2 < 0 && fp->_errfunc != NULL)
		(*fp->_errfunc)(fp->_filename, 'c', fp);
	}
else	r2 = 0;

_freefile(fp);

return (r1 == EOF || r2 < 0) ? EOF : 0;		/* ? */
}
