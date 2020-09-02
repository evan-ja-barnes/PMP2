#include <stdio.h>

fwrite(s, size, n, fp)
register char *s;
unsigned int size;
int n;
register FILE *fp;
{
register int nchars = size * n;
int r = 0;

#ifdef READWRITE

if((fp->_flag & _IORW) && (fp->_flag & (_IOREAD | _IOWRT)) == 0)
	fp->_flag |= _IOWRT;

#endif

if(!(fp->_flag & _IOWRT))
	return EOF;

#ifdef FASTFRDWR

if(fp->_base == NULL)
	_getbuf(fp);

#endif

#ifdef FASTNBF

if(fp->_flag & _IONBF)
	{
	fflush(fp);

	r = _fwrite(fp, s, nchars);

	if(r < 0)		/* if(r != nchars) ? */
		return EOF;
	}
else
#endif

#ifdef FASTFRDWR

while(nchars > 0)
	{
	register int thistime;

	if(fp->_cnt > 0 && fp->_cnt < fp->_bufsiz || nchars < fp->_bufsiz)
		{
		/* copy to buffer */

		thistime = fp->_cnt;
		if(thistime > nchars)
			thistime = nchars;

		memcpy(fp->_ptr, s, thistime);

		fp->_ptr += thistime;
		fp->_cnt -= thistime;
		s += thistime;
		nchars -= thistime;
		r += thistime;
		}

	if(nchars <= 0)
		break;

	fflush(fp);

	if(nchars >= fp->_bufsiz)
		{
		int r2;

		/* now transfer directly from user's buffer */

		while(nchars >= fp->_bufsiz)
			{
			thistime = fp->_bufsiz;

			r2 = _fwrite(fp, s, thistime);

			if(r2 < 0)		/* if(r2 != nchars) ? */
				break;

			s += r2;
			nchars -= r2;
			r += r2;
			}
		}

	/* if chars left over, next trip around loop will finish */
	}

#else

while(nchars-- > 0)
	{
	if(putc(*s++, fp) == EOF)
		break;
	r++;
	}

#endif

return r / size;
}
