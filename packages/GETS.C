#include <stdio.h>

char *
gets(s)
char *s;
{
register char *p = s;
register int c;

while((c = getchar()) != '\n' && c != EOF)
	*p++ = c;

*p = '\0';

return s;
}
