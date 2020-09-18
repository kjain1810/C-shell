#ifndef OTHER_COMMANDS
#define OTHER_COMMANDS

#include "../../utils/global.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

void backgroundProcess()
{
    pid_t flg = fork();
    if (flg < 0)
    {
        printf("%s: unable to fork\n", args[0]);
        return;
    }
    else if (flg == 0)
    {
        char **newargs = (char **)malloc(MAX_ARGUMENTS * sizeof(char *));
        int idx = 0;
        for (int a = 0; a < numargs; a++)
        {
            if (strcmp(args[a], "&") == 0)
                continue;
            newargs[idx++] = args[a];
        }
        newargs[idx] = NULL;
        if (setpgid(getpid(), 0) == -1)
        {
            perror(newargs[0]);
            exit(1);
        }
        if (execvp(newargs[0], newargs) < 0)
        {
            printf("%s: command not found\n", newargs[0]);
            exit(1);
        }
        exit(0);
    }
    else
    {
        processesID[commandCnt] = flg;
        strcpy(processesName[commandCnt], args[0]);
        printf("[%d] %d\n", ++commandCnt, flg);
    }
}

void foregroundProcess()
{
    pid_t flg = fork();
    if (flg < 0)
    {
        printf("%s: unable to fork\n", args[0]);
        return;
    }
    else if (flg == 0)
    {
        if (execvp(args[0], args) < 0)
        {
            printf("%s: command not found\n", args[0]);
            raise(SIGCHLD);
            _exit(1);
        }
        exit(0);
    }
    else
    {
        int status;
        waitpid(flg, &status, 0);
    }
}

void processEnd()
{
    for (int a = 0; a < commandCnt; a++)
    {
        int status;
        if (waitpid(processesID[a], &status, WNOHANG | WUNTRACED) > 0)
        {
            printf("[%d] %s with pid %d exited ", a + 1, processesName[a], processesID[a]);
            if (WIFEXITED(status) > 0)
                printf("normally");
            else if (WIFEXITED(status))
                printf("with status %d", status);
            else
                printf("abnormally");
            printf("\n");
        }
    }
}

void otherCommands()
{
    signal(SIGCHLD, processEnd);
    for (int a = 1; a < numargs; a++)
        if (strcmp(args[a], "&") == 0)
        {
            backgroundProcess();
            return;
        }
    foregroundProcess();
}

#endif