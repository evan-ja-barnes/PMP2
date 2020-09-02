#include <stdio.h>
#include <stdarg.h>

vscanf(fmt, argp)
char *fmt;
va_list argp;
{
return _doscan(stdin, fmt, argp);
}
