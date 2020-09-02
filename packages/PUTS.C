#include <stdio.h>

puts(s)
register char *s;
{
#ifdef FASTNBF
if(stdout->_flag & _IONBF)
	{
	char *base = s;
	while(*s != '\0')
		s++;
	_fwrite(stdout, base, s - base);
	}
else
#endif
while(*s != '\0')
	putchar(*s++);

return putchar('\n');
}
