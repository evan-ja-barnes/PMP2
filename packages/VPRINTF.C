#include <stdio.h>
#include <stdarg.h>

vprintf(fmt, argp)
char *fmt;
va_list argp;
{
return _doprnt(fmt, argp, stdout);
}
