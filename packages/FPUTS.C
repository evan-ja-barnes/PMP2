#include <stdio.h>

fputs(s, fp)
register char *s;
register FILE *fp;
{
register int r = 0;

#ifdef FASTNBF

if(fp->_flag & _IONBF)
	{
	char *base = s;

	while(*s != '\0')
		s++;

	return _fwrite(fp, base, s - base);
	}

#endif

while(*s != '\0')
	r = putc(*s++, fp);

return r;
}
