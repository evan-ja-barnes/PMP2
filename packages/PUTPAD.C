#include <stdio.h>
#include "printf.h"

/*
 *  There was a routine by this name in the 4.1bsd stdio.
 *  It probably did the same thing.
 *  This routine should therefore take the same arguments and
 *  behave the same way, if possible, except that at the moment
 *  I can't locate a copy of the old Berkeley stdio.
 */

putpad(fd, string, width, padc, flags)
FILE *fd;
char *string;
int width;
int padc;
int flags;
{
int len = strlen(string);
int i;
int retval = 0;
int padlen;

padlen = width - len;
if(padlen < 0)
	padlen = 0;

if(padlen > 0 && !(flags & PRF_LADJUST))
	{
	for(i = 0; i < padlen; i++)
		putc(padc, fd);
	retval += padlen;
	}

fputs(string, fd);
retval += len;

if(padlen > 0 && (flags & PRF_LADJUST))
	{
	for(i = 0; i < padlen; i++)
		putc(padc, fd);
	retval += padlen;
	}

return retval;
}
