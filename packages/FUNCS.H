#ifndef FUNCS_H
#define FUNCS_H

/*
 *  Linkage to underlying I/O (and memory allocation) functions.
 *  Simple changes here allow, for example, renaming read() et
 *  al to __read() etc. for strict ANSI environmentally-conscious
 *  compliance.
 */

#define OPENFUNC open
#ifndef THREEARGOPEN
#define CREATFUNC creat
#endif
#define READFUNC read
#define WRITEFUNC write
#define SEEKFUNC lseek
#define CLOSEFUNC close

#define FSTAT fstat
#define ISATTY isatty

#define MALLOC malloc
#define REALLOC realloc
#define FREE free

#ifdef __STDC__
#ifndef SAFEREALLOC
#define SAFEREALLOC	/* #define SAFEREALLOC only if realloc handles */
#endif			/* a NULL argument correctly */
#endif

extern int (*_openfunc)();
#ifndef THREEARGOPEN
extern int (*_creatfunc)();
#endif
extern int (*_readfunc)();
extern int (*_writefunc)();
extern long int (*_seekfunc)();
extern int (*_closefunc)();

extern char *(*_mallocfunc)();
extern int (*_freefunc)();
extern char *(*_reallocfunc)();

#endif
