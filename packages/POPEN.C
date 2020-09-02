#include <stdio.h>
#include "funcs.h"

#define SHELL "/bin/sh"

struct popencontext
	{
	FILE *pc_fp;
	int pc_pid;
	int pc_state;
	int pc_status;
	};

/* values for pc_state: */

#define PC_UNUSED	0
#define PC_RUNNING	1
#define PC_EXITED	2

extern char *(*_reallocfunc)();

/*
 *  Could perhaps use _fptr to point to the popen context,
 *  rather than this slightly cumbersome separate cache,
 *  but I'd like not to tie this version of popen to a
 *  particular stdio implementation.
 */

static struct popencontext *pcps = NULL;
static int npcps = 0;

static struct popencontext *
getpcp()
{
register struct popencontext *pcp;
struct popencontext *newpcps;

for(pcp = pcps; pcp < &pcps[npcps]; pcp++)
	{
	if(pcp->pc_status == PC_UNUSED)
		return pcp;
	}

npcps++;

#ifndef SAFEREALLOC
if(pcps == NULL)
	newpcps = (struct popencontext *)
			(*_mallocfunc)(npcps * sizeof(struct popencontext));
else
#endif
	newpcps = (struct popencontext *)(*_reallocfunc)((char *)pcps,
					npcps * sizeof(struct popencontext));

if(newpcps == NULL)
	return NULL;

pcps = newpcps;

return &pcps[npcps - 1];
}

FILE *
popen(command, mode)
char *command;
char *mode;
{
int pipefds[2];
struct popencontext *pcp;
int pid;
int thisend;

if((pcp = getpcp()) == NULL)
	return NULL;

if(pipe(pipefds) < 0)
	return NULL;

if((pid = fork()) < 0)
	{
	(void)close(pipefds[0]);
	(void)close(pipefds[1]);
	return NULL;
	}

/*
 *  Uncouth algorithm below lets common code for closing and
 *  dup'ing pipe be shared between parent and child.  "thisend"
 *  is the end we're interested in (read from/write to); the
 *  other end gets closed (in our process).  "thisend" is
 *  obviously opposite in parent and child.
 *
 *  Less than perfectly conceptually type-correct; assumes
 *	0 == standard input == index of read end of pipe
 *	1 == standard output == index of write end of pipe
 */

thisend = (*mode == 'r') ? 0 : 1;

if(pid == 0)
	thisend = 1 - thisend;

(void)close(pipefds[1 - thisend]);

if(pid == 0)
	{
	/*
	 *  On the off-chance that 0 or 1 had been closed (i.e.
	 *  available) before the pipe, they might now be correct,
	 *  and closing/dup'ing them would be wrong.
	 */

	if(pipefds[thisend] != thisend)
		dup2(pipefds[thisend], thisend);

	/* should it use getenv("SHELL")?  Probably not. */
	execl(SHELL, "sh", "-c", command, (char *)NULL);

	/*
	 *  It is said that stdio should be avoided in a child after a
	 *  failed exec, as should exit.  I'm not sure why, but it can't hurt.
	 */

	perror(SHELL);
	_exit(1);
	}

/* else parent */

pcp->pc_pid = pid;
pcp->pc_state = PC_RUNNING;
if((pcp->pc_fp = fdopen(pipefds[thisend], mode)) == NULL)
	{
	(void)close(pipefds[thisend]);
	/* what to do with the child? */
	/* maybe should have fdopened before fork */
	return NULL;
	}

return pcp->pc_fp;
}

static struct popencontext *
findpcp(fp)
FILE *fp;
{
register struct popencontext *pcp;

for(pcp = pcps; pcp < &pcps[npcps]; pcp++)
	{
	if(pcp->pc_fp == fp)
		return pcp;
	}

return NULL;
}

pclose(fp)
FILE *fp;
{
struct popencontext *pcp;

fclose(fp);

if((pcp = findpcp(fp)) == NULL)
	{
	/* complain anout popen botch? */
	return -1;
	}

if(pcp->pc_state == PC_RUNNING)
	{
	int status;
	int w;

	while((w = wait(&status)) >= 0)
		{
		if(w == pcp->pc_pid)
			{
			pcp->pc_status = status;
			break;
			}
		else	{
			struct popencontext *pcp2;

			for(pcp2 = pcps; pcp2 < &pcps[npcps]; pcp2++)
				{
				if(pcp2->pc_pid == w)
					{
					pcp2->pc_status = status;
					pcp2->pc_state = PC_EXITED;
					break;
					}
				}
			}
		}
	}

pcp->pc_state = PC_UNUSED;	/* so getpcp will find it */
pcp->pc_pid = -1;		/* so wait loop won't find it */
pcp->pc_fp = NULL;		/* so findpcp won't find it */

return pcp->pc_status;
}
