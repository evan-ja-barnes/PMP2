#include <stdio.h>
#include "funcs.h"

#define TRUE 1
#define FALSE 0

FILE *_ioblist = NULL;

extern FILE *_lastiob;

FILE *_getfile();

extern (*_deflterrfunc)();

FILE *
fopen(filename, mode)
char *filename;
char *mode;
{
register FILE *fp;
register int fd;
int flag;
char rwamode;
char *p;
#ifdef READWRITE
int rwflag = FALSE;
#endif

rwamode = *mode;

/*
 *  Should the flags really be accepted in any order?  H & S say
 *  only that "...any of the types ["r", "w+", etc.] [can] be
 *  followed by the character b..."
 */

for(p = &mode[1]; *p != '0'; p++)
	{
	switch(*p)
		{
#ifdef READWRITE
		case '+':
			rwflag = TRUE;
			break;
#endif
#ifdef notyet
		case 'b':
			binary = TRUE;
			break;
#endif
		/* 
		 *  Note that unrecognized mode modifiers are _i_g_n_o_r_e_d.
		 *  This is desirable; semi-standard extensions to stdio
		 *  are common, but not usually relevant on Unix systems.
		 */
		}
	}

if(rwamode == 'r')
	{
#ifdef READWRITE
	if(rwflag)
		{
		if((fd = (*_openfunc)(filename, 2)) < 0)
			return NULL;
		flag = _IORW;
		}
	else
#endif
		{
		if((fd = (*_openfunc)(filename, 0)) < 0)
			return NULL;
		flag = _IOREAD;
		}
	}
else	{
#ifdef READWRITE
	if(rwflag)
		{
		if(rwamode == 'w' || (fd = (*_openfunc)(filename, 2) < 0))
			fd = (*_creatfunc)(filename, 0666);

		if(fd < 0)
			return NULL;

		if((*_closefunc)(fd) < 0 ||
				(fd = (*_openfunc)(filename, 2)) < 0)
			return NULL;

		if(rwamode == 'a')
			(*_seekfunc)(fd, 0L, 2);

		flag = _IORW;
		}
	else
#endif
		{
		if(rwamode == 'w' || (fd = (*_openfunc)(filename, 1) < 0))
			fd = (*_creatfunc)(filename, 0666);

		if(fd < 0)
			return NULL;

		if(rwamode == 'a')
			(*_seekfunc)(fd, 0L, 2);

		flag = _IOWRT;
		}
	}

if((fp = _getfile()) != NULL)
	{
	fp->_file = fd;
	fp->_flag |= flag;
	if((fp->_filename = (*_mallocfunc)(strlen(filename) + 1)) != NULL)
		strcpy(fp->_filename, filename);
	}

return fp;
}

FILE *
_getfile()
{
register FILE *fp;

for(fp = _iob; fp < _lastiob; fp++)
	{
	if(fp->_flag == 0)
		{
		_initfile(fp);
		return fp;
		}
	}

if((fp = (FILE *)(*_mallocfunc)(sizeof(FILE))) != NULL)
	{
	_initfile(fp);

	fp->_next = _ioblist;	/* would be nicer to keep FIFO */
	_ioblist = fp;
	}
	/*
	 *  ...else set errno?  (Generally, library functions
	 *  shouldn't, but the caller is likely to call perror
	 *  after f*open returns NULL, and EMFILE is germane.)
	 */

return fp;
}

_freefile(fp)
register FILE *fp;
{
register FILE **fpp;

if(fp->_base != NULL && (fp->_flag & _IOMYBUF))
	{
	(*_freefunc)(fp->_base);
	fp->_base = NULL;
	}

if(fp->_filename != NULL && !(fp->_flag & _IOSTFN))
	{
	(*_freefunc)(fp->_filename);
	fp->_filename = NULL;
	}

if(fp >= _iob && fp < _lastiob)		/* potentially unportable */
	{					/* pointer comparison */
	fp->_flag = 0;
	return;
	}

for(fpp = &_ioblist; *fpp != NULL; fpp = &(*fpp)->_next)
	{
	if(*fpp == fp)
		{
		*fpp = fp->_next;
		break;
		}
	}

/* what if it wasn't found? */

(*_freefunc)((char *)fp);
}

_initfile(fp)
register FILE *fp;
{
fp->_base = fp->_ptr = fp->_filename = NULL;
fp->_cnt = fp->_flag = 0;
fp->_bufsiz = BUFSIZ;	/* could be 0; caller will usually fix;
						but 0 is too dangerous */
fp->_filbuf = NULL;
fp->_flsbuf = NULL;
fp->_readfunc = _readfunc;
fp->_writefunc = _writefunc;
fp->_seekfunc = _seekfunc;
fp->_closefunc = _closefunc;
fp->_errfunc = _deflterrfunc;

/* _next not set: would break freopen */
}
