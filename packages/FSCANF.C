#include <stdio.h>
#include <stdarg.h>

/* VARARGS2 */

fscanf(fp, fmt)
FILE *fp;
char *fmt;
{
register va_list argp;
int r;

va_start(argp, fmt);
r = _doscan(fp, fmt, argp);
va_end(argp);
return r;
}
