#include <stdio.h>
#include <stdarg.h>

/* VARARGS2 */

sscanf(s, fmt)
char *s;
char *fmt;
{
FILE f;
register va_list argp;
int r;

_initsfile(&f);		/* might force loading of sprintf.o */

f._base = f._ptr = s;
f._cnt = f._bufsiz = strlen(s);
f._flag |= _IOREAD;

va_start(argp, fmt);

r = _doscan(&f, fmt, argp);

va_end(argp);

return r;
}
