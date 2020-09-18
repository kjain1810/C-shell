#ifndef GETTING_INPUT
#define GETTING_INPUT

#include "global.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *readLine()
{
    char *ret = (char *)malloc(INPUT_LENGTH * sizeof(char));

    if (!ret)
    {
        perror("Memory allocation failure");
        exit(1);
    }

    size_t len = INPUT_LENGTH;
    getline(&ret, &len, stdin);

    return ret;
}

char **separateInput(char *line)
{
    char **ret = (char **)malloc(MAX_ARGUMENTS * sizeof(char *));
    char *arg;
    int idx = 0;
    if (!ret)
    {
        perror("Memory allocation failure");
        exit(1);
    }
    arg = strtok(line, DELIMITER);
    while (arg != NULL)
    {
        ret[idx++] = arg;
        arg = strtok(NULL, DELIMITER);
    }
    ret[idx] = arg;
    numargs = idx;
    return ret;
}

void separateMultiArgs(char *inp)
{
    int idx = 0;
    char *here = strtok(inp, ";");
    while (here != NULL)
    {
        multiargs[idx++] = here;
        here = strtok(NULL, ";");
    }
    numCommands = idx;
    multiargs[numCommands] = NULL;
}

#endif