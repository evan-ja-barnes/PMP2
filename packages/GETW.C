#include <stdio.h>

getw(fp)
register FILE *fp;
{
int ret = 0;
int c;
int i;

/*
 *  Could presumably use sizeof(int) instead of 2,
 *  but I think getw reads 2 bytes even on 32-bit machines.
 */

/* Assumes little-endian ints, I think */

for(i = 0; i < 2; i++)
	{
	if((c = getc(fp)) == EOF)	/* what about any bytes read during */
		return EOF;		/* prior trip(s) through loop? */

	((char *)&ret)[i] = c;
	}

return ret;
}
