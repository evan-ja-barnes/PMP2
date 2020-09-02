#include <stdio.h>
#include <stdarg.h>

vfscanf(fp, fmt, argp)
FILE *fp;
char *fmt;
va_list argp;
{
return _doscan(fp, fmt, argp);
}
