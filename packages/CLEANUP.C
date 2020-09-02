extern int fflush();
extern int fclose();

_cleanup()
{
#ifdef CLEANUPCLOSE
_scanfiles(fclose);
#else
_scanfiles(fflush);
#endif
}
