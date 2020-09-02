#include <stdio.h>

rewind(fp)
FILE *fp;
{
fseek(fp, 0L, SEEK_SET);

/*
 *  The standard implementation inexplicably clears _IOERR, which
 *  contradicts the assertion of ferror(3S) that "unless cleared by
 *  clearerr, the error indication lasts until the stream is closed."
 *  H & S agree; K & R 2nd Ed. document an implicit clearerr.
 */
}
