/*
 *  char *saprintf(fmt, args)
 *
 *  Like sprintf, but malloc's its own buffer.
 *  Returns pointer to malloc'ed memory just big enough to hold
 *  formatted string.
 *  Caller must free this memory if/when no longer needed.
 */

#include <stdio.h>
#include <stdarg.h>

extern char *_safinish();

char *
saprintf(fmt)
char *fmt;
{
va_list argp;
FILE f;

_initsafile(&f);

va_start(argp, fmt);

_doprnt(fmt, argp, &f);

va_end(argp);

putc('\0', &f);

return _safinish(&f);
}
