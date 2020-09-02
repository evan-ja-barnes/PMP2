#include <stdio.h>
#include <stdarg.h>

askf(prompt)
char *prompt;
{
va_list argp;
char *fmt;
int r;

do	{
	va_start(argp, prompt);

	/*
	 *  Have to pass argp by reference so _doscan can pick up
	 *  where _doprnt left off.
	 */

	_do2prnt(1, prompt, &argp, stderr);

	fmt = va_arg(argp, char *);

	r = _do2scan(1, stdin, fmt, argp);

	va_end(argp);
	} while(r < 0);

return r;
}
