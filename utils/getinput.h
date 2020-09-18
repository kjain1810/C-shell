#ifndef GETTING_INPUT
#define GETTING_INPUT

#include "global.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

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

int parseInputFiles()
{
    int idx = 0;
    for (int a = 1; a < numargs; a++)
    {
        if (args[a][0] == '<')
        {
            idx = a;
            break;
        }
    }
    if (!idx)
        return 1;
    char *fileName = (char *)malloc(INPUT_LENGTH * sizeof(char));
    if (strlen(args[idx]) == 1)
    {
        strcpy(fileName, args[idx + 1]);
        for (int a = idx + 2; a < numargs; a++)
            args[a - 2] = args[a];
        args[numargs - 1] = NULL;
        args[numargs - 2] = NULL;
        numargs -= 2;
    }
    else
    {
        strcpy(fileName, args[idx] + 1);
        for (int a = idx + 1; a < numargs; a++)
            args[a - 1] = args[a];
        args[numargs - 1] = NULL;
        numargs -= 1;
    }
    int fd = open(fileName, O_RDONLY);
    if (fd == -1)
    {
        perror("File error");
        return 0;
    }
    copyOfStdIn = dup(0);
    dup2(fd, 0);
    changedStdIn = 1;
    return 1;
}

void resetInputFile()
{
    dup2(copyOfStdIn, 0);
    changedStdIn = 0;
}

void parseOutputFiles()
{
    int idx = 0;
    for (int a = 1; a < numargs; a++)
    {
        if (args[a][0] == '>')
        {
            idx = a;
            break;
        }
    }
    if (!idx)
        return;
    char *fileName = (char *)malloc(INPUT_LENGTH * sizeof(char));
    int overWrite = 0;
    if (strlen(args[idx]) == 1)
    {
        overWrite = 1;
        strcpy(fileName, args[idx + 1]);
        for (int a = idx + 2; a < numargs; a++)
            args[a - 2] = args[a];
        args[numargs - 1] = NULL;
        args[numargs - 2] = NULL;
        numargs -= 2;
    }
    else if (strlen(args[idx]) > 2)
    {
        if (args[idx][1] == '>')
            strcpy(fileName, args[idx] + 2);
        else
        {
            overWrite = 1;
            strcpy(fileName, args[idx] + 1);
        }
        for (int a = idx + 1; a < numargs; a++)
            args[a - 1] = args[a];
        args[numargs - 1] = NULL;
        numargs -= 1;
    }
    else
    {
        if (args[idx][1] == '>')
        {
            strcpy(fileName, args[idx + 1]);
            for (int a = idx + 2; a < numargs; a++)
                args[a - 2] = args[a];
            args[numargs - 1] = NULL;
            args[numargs - 2] = NULL;
            numargs -= 2;
        }
        else
        {
            overWrite = 1;
            strcpy(fileName, args[idx] + 1);
            for (int a = idx + 1; a < numargs; a++)
                args[a - 1] = args[a];
            args[numargs - 1] = NULL;
            numargs -= 1;
        }
    }
    int fd;
    if (overWrite)
        fd = open(fileName, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    else
        fd = open(fileName, O_CREAT | O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd == -1)
    {
        perror("File error");
        return;
    }
    copyOfStdout = dup(1);
    dup2(fd, 1);
    changedStdOut = 1;
}

void resetOutputfile()
{
    dup2(copyOfStdout, 1);
    changedStdOut = 0;
}

#endif