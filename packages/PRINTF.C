#include <stdio.h>
#include <stdarg.h>

/* VARARGS1 */

printf(fmt)
char *fmt;
{
register va_list argp;
int r;

va_start(argp, fmt);
r = _doprnt(fmt, argp, stdout);
va_end(argp);
return r;
}
