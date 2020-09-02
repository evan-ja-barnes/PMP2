#include <stdio.h>

fdclose(fp)
register FILE *fp;
{
int r;

r = fflush(fp);

_freefile(fp);

return r;
}
