#include <stdio.h>
#include <stdarg.h>

/* VARARGS1 */

scanf(fmt)
char *fmt;
{
register va_list argp;
int r;

va_start(argp, fmt);
r = _doscan(stdin, fmt, argp);
va_end(argp);
return r;
}
