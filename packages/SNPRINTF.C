#include <stdio.h>
#include <stdarg.h>

snprintf(s, n, fmt)
char *s;
int n;
char *fmt;
{
va_list argp;
FILE f;
int r;

_initsfile(&f);

f._base = f._ptr = s;
f._cnt = n - 1;			/* leave room for \0 */
f._flag |= _IOWRT;

va_start(argp, fmt);

r = _doprnt(fmt, argp, &f);

va_end(argp);

*f._ptr++ = '\0';

return r;
}
