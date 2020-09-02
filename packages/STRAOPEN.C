#include <stdio.h>

extern FILE *_getfile();
extern char *_safinish();

FILE *
straopen()
{
register FILE *fp;

if((fp = _getfile()) == NULL)
	return NULL;

_initsafile(fp);

return fp;
}

char *
straptr(fp)
register FILE *fp;
{
if(!(fp->_flag & _IOSTRG))	/* imperfect */
	return NULL;

putc('\0', fp);

fp->_ptr--;	/* so \0 will be overwritten next time */
fp->_cnt++;

return fp->_ptr;
}

char *
straclose(fp)
register FILE *fp;
{
char *ret;

if(!(fp->_flag & _IOSTRG))	/* imperfect */
	return NULL;

putc('\0', fp);

ret = _safinish(fp);

fp->_base = NULL;	/* just in case _freefile ever feels like freeing */

_freefile(fp);

return ret;
}
