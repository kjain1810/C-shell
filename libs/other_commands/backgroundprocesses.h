#ifndef BACKGROUND_PROCESSES
#define BACKGROUND_PROCESSES

#include "../../utils/global.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int backgroundProcess()
{
    pid_t flg = fork();
    if (flg < 0)
    {
        printf("%s: unable to fork\n", args[0]);
        return 0;
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
        fprintf(stderr, "[%d] %d\n", ++commandCnt, flg);
        return 1;
    }
}

#endif