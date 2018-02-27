
#ifndef JOBS_H
#define JOBS_H

#include	<string.h>
#define MAX_PID_DIGITS 5

struct job
{
	int jobNumber;
	pid_t pid;
	char* command;
	int stat;
};

typedef struct job *JobPtr;
typedef struct job Job;
struct job* createJob(int, pid_t, char*, int);
int jobDone(const void *jobObj);
void printJobStatus(const void *jobObj);
void printJob(const void *jobObj);
void addJob(pid_t pid, char *unmodLine, struct list *list, int status);
void freeJob(void *);
char *jobToString(const void *);
int jobEqual(const void *,const void *);








#endif /* __OBJECT_H */
