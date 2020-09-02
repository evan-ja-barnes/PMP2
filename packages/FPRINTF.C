#include <stdio.h>
#include <stdarg.h>

/* VARARGS2 */

fprintf(fp, fmt)
FILE *fp;
char *fmt;
{
register va_list argp;
int r;

va_start(argp, fmt);
r = _doprnt(fmt, argp, fp);
va_end(argp);
return r;
}
