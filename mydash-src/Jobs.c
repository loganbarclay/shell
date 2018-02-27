
#include	"mydash.h"


struct job* createJob(int number, pid_t id, char* comm, int status) {

	struct job* tempJob = (struct job*) malloc (sizeof(struct job));
	tempJob->jobNumber = number;
	tempJob->pid = id;
	tempJob->command = (char *) malloc(sizeof(char) * (strlen(comm) + 1));
	tempJob->stat = status;
	strcpy(tempJob->command, comm);
	return tempJob;
}

/**
 * @brief      ToString of a job object
 *
 * @param[in]  jobObj  The job object
 *
 * @return     The string representation of the job
 */
char *jobToString(const void *jobObj) {

	struct job* myJob = (struct job*) jobObj;
	char *temp;
	int max_comm = strlen(myJob->command) + 1;
	max_comm += MAX_PID_DIGITS;
	temp = (char *)malloc(max_comm);
	//print into our buffer safely
	snprintf(temp, max_comm, "[%d] %s", myJob->pid, myJob->command);
	return temp;
}

/**
 * @brief      Frees the job object from the list
 *
 * @param      jobObj  The job object
 */
void freeJob(void *jobObj) {

	struct job* myJob = (struct job*) jobObj;
	if (myJob == NULL) return;
	free(myJob->command);
	free(myJob);
}

/**
 * @brief      Prints the specified job
 *
 * @param[in]  jobObj  The job object
 */
void printJob(const void *jobObj) {

	JobPtr object = (JobPtr) jobObj;
	if(object->stat == 0){
		printf("[%d] Stopped   %s \n", object->jobNumber, object->command);
	}else{
	printf("[%d] %d %s \n", object->jobNumber, (int) object->pid, object->command);
	}
}

/**
 * @brief      Checks if the job is the right one
 *
 * @param[in]  object  The job object
 * @param[in]  other   The other job object
 *
 * @return     returns the pid if they are equal
 */
int jobEqual(const void *object, const void *other) {

	struct job* j1 = (struct job*) object;
	struct job* j2 = (struct job*) other;
	return j1->pid == j2->pid;
}

/**
 * @brief      Prints the status of the job, "running", "stopped", or "done"
 *
 * @param[in]  jobObj  The job object
 */
void printJobStatus(const void *jobObj) {

	JobPtr object = (JobPtr) jobObj;
	if (jobDone(jobObj)) {

		printf("[%d] Done %s \n", object->jobNumber, object->command);
	}
	if (object->stat == 1) {

		printf("[%d] %d Running %s \n", object->jobNumber, (int) object->pid, object->command);
	}
	else if (object->stat == 0) {

		printf("[%d] %d Stopped %s \n", object->jobNumber, (int) object->pid, object->command);
	}

}

/**
 * @brief      Checks if a job is done
 *
 * @param[in]  jobObj  The job object
 *
 * @return     returns if job is done
 */
int jobDone(const void *jobObj) {

	JobPtr object = (JobPtr) jobObj;
	int statusPtr;
	return waitpid(object->pid, &statusPtr, WNOHANG);
}

/**
 * @brief      Prints the done jobs
 *
 * @param      list  The list of jobs
 */
void printDoneJobs(struct list *list) {

	if (list->size == 0) {
		return;
	}

	NodePtr temp = list->head;


	while (temp != NULL) {
		if (jobDone(temp->obj)) {
			NodePtr nxtNode = temp->next;
			printJobStatus(temp->obj);
			removeNode(list, temp);
			freeNode(temp, list->freeObject);
			temp = nxtNode;

		} else {
			temp = temp->next;
		}
	}

}

/**
 * @brief      Prints all jobs
 *
 * @param      list  The list of jobs
 */
void printJobs(struct list *list) {

	if (list->size == 0)
		return;

	NodePtr temp = list->head;

	while (temp != NULL) {
		printJobStatus(temp->obj);
		temp = temp->next;

	}

}

/**
 * @brief      Adds a job.
 *
 * @param[in]  pid        The pid of the job
 * @param      unmodLine  The unmodified command
 * @param      list       The list of jobs
 * @param[in]  status     The status of the job
 */
void addJob(pid_t pid, char *unmodLine, struct list *list, int status) {

	int num = (list->size == 0) ? 1 : ((JobPtr) (list->tail->obj))->jobNumber + 1;

	JobPtr tempJob = createJob(num, pid, unmodLine, status);

	NodePtr node = createNode(tempJob);

	addAtRear(list, node);
	char *tempString = jobToString(tempJob);
	printJob(tempJob);
	free(tempString);
}