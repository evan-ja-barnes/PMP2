#include <stdio.h>
#include <stdarg.h>

/* VARARGS2 */

sprintf(s, fmt)
char *s;
char *fmt;
{
va_list argp;
FILE f;
int r;

_initsfile(&f);

f._base = f._ptr = s;
f._cnt = 32767;				/* oughta be INT_MAX or something */
f._flag |= _IOWRT;

va_start(argp, fmt);

r = _doprnt(fmt, argp, &f);

va_end(argp);

putc('\0', &f);

return r;
}
