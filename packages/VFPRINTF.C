#include <stdio.h>
#include <stdarg.h>

vfprintf(fd, fmt, argp)
FILE *fd;
char *fmt;
va_list argp;
{
return _doprnt(fmt, argp, fd);
}
