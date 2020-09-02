#include <stdio.h>
#include <stdarg.h>

vsprintf(s, fmt, argp)
char *s;
char *fmt;
va_list argp;
{
FILE f;
int r;

f._base = f._ptr = s;
f._cnt = 32767;				/* oughta be INT_MAX or something */
f._flag = _IOWRT | _IOSTRG;

/* _IOSTRG should keep _writefunc from getting called, but just in case: */

f._writefunc = NULL;
f._file = -1;

r = _doprnt(fmt, argp, &f);

putc('\0', &f);

return r;
}
