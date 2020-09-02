#include <stdio.h>

char *
fgets(s, n, fp)
char *s;
int n;
FILE *fp;
{
register char *p = s;
register int c;

while(--n > 0)
	{
	c = getc(fp);

	if(c == EOF)
		break;

	*p++ = c;
	n--;

	if(c == '\n')
		break;
	}

*p = '\0';

return s;
}
