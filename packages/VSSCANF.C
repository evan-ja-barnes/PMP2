#include <stdio.h>
#include <stdarg.h>

vsscanf(s, fmt, argp)
char *s;
char *fmt;
va_list argp;
{
FILE f;

f._base = f._ptr = s;
f._cnt = f._bufsiz = strlen(s);
f._flag |= _IOREAD | _IOSTRG;

/* _IOSTRG should keep _writefunc from getting called, but just in case: */

f._readfunc = NULL;
f._file = -1;

return _doscan(&f, fmt, argp);
}
