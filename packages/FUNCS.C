#include "funcs.h"

extern int OPENFUNC();
#ifndef THREEARGOPEN
extern int CREATFUNC();
#endif
extern int READFUNC();
extern int WRITEFUNC();
extern long int SEEKFUNC();
extern int CLOSEFUNC();

extern char *MALLOC();
extern FREE();

int (*_openfunc)() = OPENFUNC;
#ifndef THREEARGOPEN
int (*_creatfunc)() = CREATFUNC;
#endif
int (*_readfunc)() = READFUNC;
int (*_writefunc)() = WRITEFUNC;
long int (*_seekfunc)() = SEEKFUNC;
int (*_closefunc)() = CLOSEFUNC;

char *(*_mallocfunc)() = MALLOC;
int (*_freefunc)() = FREE;
