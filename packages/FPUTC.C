#include <stdio.h>

fputc(c, fp)
char c;
FILE *fp;
{
return putc(c, fp);
}
