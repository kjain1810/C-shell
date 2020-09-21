#ifndef FOREGROUND_PROCESSES
#define FOREGROUND_PROCESSES

#include "../../utils/global.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

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
        curForegroundProcess = flg;
        int status;
        int ret = waitpid(flg, &status, WUNTRACED);
        curForegroundProcess = shellPID;
        if (ret == -1)
            exit_status[2] = '(';
        else
            exit_status[2] = ')';
    }
}

#endif