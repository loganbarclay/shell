
#include    "mydash.h"


/**
 * @brief      Signal handler that catches ctrl-c and ctrl-z
 *
 * @param[in]  signo  The signal that was caught
 */
void onintr(int signo) {
	initSignal();
	switch (signo) {
	case SIGINT:
		printf("\nCaught Ctrl-C\n");
		siglongjmp(point, FROM_ONINTR);
		break;
	case SIGTSTP:
		printf("\nCaught Ctrl-Z\n");
		siglongjmp(point, FROM_ONINTR);
		break;
	default: break;
	}
}

/**
 * @brief      Executes the processes
 *
 * @param      token       The token array from the command line
 * @param[in]  background  The background flag
 * @param      unmodLine   The unmodified command
 * @param      list        The list of jobs
 */
void execute(char **token, int background, char *unmodLine, struct list *list) {

	pid_t pid;
	int status;
	char* cmd = unmodLine;


	if ( (pid = fork()) < 0) {
		err_sys("fork error");
	}
	else if (pid == 0) {
		if (background) {
			ignoreSignal();
		}
		if (execvp(*token, token) < 0) {
			err_ret("couldn't execute: %s", token);
			exit(EXIT_FAILURE);
		}
	}

	else {
		if (!background) {
			ignoreSignal();
			if ( (pid = waitpid(pid, &status, WUNTRACED)) < 0)
				err_sys("waitpid error");
			else if (WIFSTOPPED(status)) {
				addJob(pid, cmd, list, 0);
			}
		} else {
			addJob(pid, unmodLine, list, 1);
			
		}
	}
	initSignal();
}

/**
 * @brief      Changes the directory
 *
 * @param      token  The token that holds the destination directory
 */
void chgDir(char **token) {

	DIR* dir = opendir(token[1]);
	if (dir) {
		chdir(token[1]);
	} else if (ENOENT == errno) {

		printf("Directory does not exist\n");

	} else {
		struct passwd *pw = getpwuid(getuid());
		const char *homedir = pw->pw_dir;
		chdir(homedir);
	}
}

/**
 * @brief      Evaluates the first token from the command line
 *
 * @param      token  The token to be evaluated
 * @param      line   The line entered
 * @param      list   The list of jobs
 *
 * @return     returns 1 for command line arguments, 2 for exit arguments
 */
int tokenEval(char **token, char *line, struct list *list) {

	if (token[0] == NULL) {
		return 1;
	}

	if (strcmp(token[0], "jobs") == 0) {
		printJobs(list);
		return 1;
	}

	if (strcmp(token[0], "exit") == 0) {
		printf("mydash shell closed\n");
		return 2;
	}

	if (strcmp(token[0], "cd") == 0) {
		chgDir(token);
		return 1;
	}

	if (strcmp(token[0], "-v") == 0) {
		printf("Version: %s \n", git_version());
		return 2;
	}
	return 0;


}


