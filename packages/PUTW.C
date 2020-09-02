#include <stdio.h>

putw(w, fp)
int w;
register FILE *fp;
{
int i;

/*
 *  Could presumably use sizeof(int) instead of 2,
 *  but I think putw writes 2 bytes even on 32-bit machines.
 */

/* Assumes little-endian ints, I think */

for(i = 0; i < 2; i++)
	putc(((char *)&w)[i], fp);

return w;
}
