#ifndef	__MYDASH_H
#define	__MYDASH_H

#include	<sys/types.h>
#include	<sys/wait.h>	/* required for some of our prototypes */
#include	<stdio.h>		/* for convenience */
#include	<stdlib.h>
#include 	<signal.h>
#include	<sysexits.h>
#include	<string.h>
#include	<unistd.h>
#include	<pwd.h>
#include	<dirent.h>
#include	<errno.h>
#include 	<setjmp.h>
#include	<readline/readline.h>
#include	<readline/history.h>
#include    "../../p0/include/List.h"
#include    "../../p0/include/Node.h"
#include    "Jobs.h"
#include    "parser.h"
#include    "commands.h"

jmp_buf point;

#define TRUE 1

#define FROM_ONINTR 333333

#define	MAXLINE	4096			/* max line length */

#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
/* default file access permissions for new files */
#define	DIR_MODE	(FILE_MODE | S_IXUSR | S_IXGRP | S_IXOTH)
/* default permissions for new directories */
//extern char *DASH_PROMPT;

/* prototypes for our own functions */

void	err_dump(const char *, ...);	/* {App misc_source} */
void	err_msg(const char *, ...);
void	err_quit(const char *, ...);
void	err_ret(const char *, ...);
void	err_sys(const char *, ...);

void	initSignal();
void	ignoreSignal();
void	onintr(int signo);

#endif	/* __MYDASH_H */
