#ifndef COMMANDS_H
#define COMMANDS_H

#include    <stdlib.h>
#include    <stdio.h>
#include	<string.h>
#include    "../../p0/include/List.h"
#include    "../../p0/include/Node.h"



void execute(char **token, int background, char *unmodLine, struct list *list);
void chgDir(char **token);
void printDoneJobs(struct list *list);
void printJobs(struct list *list);
const char* git_version(void);
int tokenEval(char **token, char *line, struct list *list);


#endif /* COMMANDS_H */