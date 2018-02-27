
#include    "mydash.h"

const int MAX_ARGS = 2049;


/**
 * @brief      Parses the command line input into tokens
 *
 * @param      s          The command entered into the shell
 * @param      delimiter  The delimiter
 * @param      numTokens  The number of tokens
 *
 * @return     Returns an array of tokens
 */
char **parseInput(char *s, char *delimiter, int *numTokens) {

    char **token;
    char *nextToken;

    token = (char **) calloc(MAX_ARGS, sizeof(char *));
    /* tokenize the string s */
    nextToken = strtok(s, delimiter);
    *numTokens = 0;
    while (nextToken != NULL) {
        token[*numTokens] = (char *) malloc(sizeof(char) * (strlen(nextToken) + 1));
        memset(token[*numTokens], 0, (strlen(nextToken) + 1));
        strcpy(token[*numTokens], nextToken);
        (*numTokens)++;
        nextToken = strtok(NULL, delimiter);


    }
    /*Now the tokens are copied into token[0..numTokens-1];*/
    return token;
}

/**
 * @brief      Frees the tokens from the parsed list
 *
 * @param      token  array of tokens
 */
void freeParsedList(char **token) {

    int i = 0;

    for (i = 0; i < MAX_ARGS; i++) {

        free(token[i]);
    }

    free(token);
}
