/*
 *  char *vsaprintf(fmt, va_list)
 *
 *  Like vsprintf, but malloc's its own buffer.
 *  Returns pointer to malloc'ed memory just big enough to hold
 *  formatted string.
 *  Caller must free this memory if/when no longer needed.
 */

#include <stdio.h>
#include <stdarg.h>

extern char *(*_reallocfunc)();

extern int _saflsbuf();

char *
vsaprintf(fmt, argp)
char *fmt;
va_list argp;
{
FILE f;

_initsfile(&f);

f._base = f._ptr = NULL;
f._cnt = f._bufsiz = 0;
f._flag |= _IOWRT;
f._flsbuf = _saflsbuf;

_doprnt(fmt, argp, &f);

putc('\0', &f);

return (*_reallocfunc)(f._base, f._bufsiz - f._cnt);
}
