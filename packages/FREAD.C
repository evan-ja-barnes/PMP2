#include <stdio.h>

fread(s, size, n, fp)
register char *s;
unsigned int size;
int n;
register FILE *fp;
{
register int nchars = size * n;
register int c;
int r = 0;

#ifdef READWRITE

if((fp->_flag & _IORW) && (fp->_flag & (_IOREAD | _IOWRT)) == 0)
	fp->_flag |= _IOREAD;

#endif

if(!(fp->_flag & _IOREAD))
	return EOF;

#ifdef FASTNBF

if(fp->_flag & _IONBF)
	{
	if(fp->_cnt > 0)
		{
		*s++ = *fp->_ptr++;
		fp->_cnt--;
		s++;
		nchars--;
		r++;
		}

	if(nchars > 0)
		{
		int r2;

		r2 = _fread(fp, s, nchars);

		if(r2 > 0)
			r += r2;
		}
	}
else
#endif

#ifdef FASTFRDWR

while(nchars > 0)
	{
	register int thistime;

	if(fp->_cnt > 0)
		{
		/* first finish emptying buffer */

		thistime = fp->_cnt;
		if(thistime > nchars)
			thistime = nchars;

		memcpy(s, fp->_ptr, thistime);

		fp->_ptr += thistime;
		fp->_cnt -= thistime;
		s += thistime;
		nchars -= thistime;
		r += thistime;
		}

	while(nchars >= fp->_bufsiz)
		{
		int r2;

		r2 = _fread(fp, s, fp->_bufsiz);

		if(r2 <= 0)
			break;

		s += r2;
		nchars -= r2;
		r += r2;
		}

	if(fp->_flag & _IOEOF)
		break;

	/* finally, fill stdio buffer... */

	if(nchars > 0)
		{
		if((c = _filbuf(fp)) == EOF)
			break;

		*s++ = c;
		nchars--;
		r++;
		}

	/* next time around loop will transfer remaining */
	}

#else

while(nchars-- > 0)
	{
	if((c = _filbuf(fp)) == EOF)
		break;

	*s++ = c;
	r++;
	}

#endif

return r / size;
}
