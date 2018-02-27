/*
  This program needs the file error.c and ourhdr.h to compile.
 */
#include    "mydash.h"


/**
 * @brief      main driver
 *
 * @return     returns 0 if exits normally
 */
int main(void) {

    char    *unmodLine = "";
    char    *line;
    char    *env = getenv("DASH_PROMPT");
    char    *DASH_PROMPT = env == NULL ? "mydash>" : env;
    struct  list *list;
    int     numTokens = 0;
    int     backgroundCheck = 0;
    int     tokenNum;
    char    **token;
    char    *delimiter = " ";

    using_history();

    initSignal();

    list = createList(&jobEqual, &jobToString, &freeJob);

    sigsetjmp(point, TRUE);

    while ((line = readline(DASH_PROMPT))) {

        printDoneJobs(list);

        if (*line) {
            add_history(line);
            unmodLine = malloc(sizeof(char) * (strlen(line) + 1));
            strcpy(unmodLine, line);
            int i = 1;
            while (isspace(line[strlen(line) - i])) {
                i++;
            }
            if (line[strlen(line) - i] == '&') {
                backgroundCheck = 1;

                line[strlen(line) - i] = '\0';
            }

            token = parseInput(line, delimiter, &numTokens);
            tokenNum = tokenEval(token, line, list);
            
            if (tokenNum == 1) {
                freeParsedList(token);
                free(line);
                free(unmodLine);
                continue;
            }

            else if (tokenNum == 2) {
                freeParsedList(token);
                free(line);
                freeList(list);
                free(unmodLine);
                exit(EXIT_SUCCESS);
            }

            execute(token, backgroundCheck, unmodLine, list);
            free(line);
            freeParsedList(token);
            free(unmodLine);
            backgroundCheck = 0;
        }

    }
    exit(EXIT_SUCCESS);

}

/**
 * @brief      Initializes the signal handlers for ctrl-c and ctrl-z
 */
void initSignal() {
    if (signal(SIGINT, onintr) == SIG_ERR)
        printf("Error in setting up interrupt handler\n");
    if (signal(SIGTSTP, onintr) == SIG_ERR)
        printf("Error in setting up interrupt handler\n");
}

/**
 * @brief      Ignores the ctrl-c and ctrl-z signals
 */
void ignoreSignal() {
    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
}