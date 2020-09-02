#include <stdio.h>

setbuffer(fp, buf, size)
FILE *fp;
char *buf;
int size;
{
return setvbuf(fp, buf, buf != NULL || size != 0 ? _IOFBF : _IONBF, size);
}
