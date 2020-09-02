extern int fflush();

/* actually, this is identical to cleanup() */

flushall()
{
_scanfiles(fflush);
}
