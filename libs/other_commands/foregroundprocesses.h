#ifndef FOREGROUND_PROCESSES
#define FOREGROUND_PROCESSES

#include "../../utils/global.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int foregroundProcess()
{
    pid_t flg = fork();
    if (flg < 0)
    {
        printf("%s: unable to fork\n", args[0]);
        return 0;
    }
    else if (flg == 0)
    {
        args[numargs] = NULL;
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
        signal(SIGTTOU, SIG_IGN);
        signal(SIGTTIN, SIG_IGN);
        tcsetpgrp(0, getpgid(flg));
        int ret = waitpid(flg, &status, WUNTRACED);
        tcsetpgrp(0, getpgid(shellPID));
        signal(SIGTTOU, SIG_DFL);
        signal(SIGTTIN, SIG_DFL);
        curForegroundProcess = shellPID;
        if (ret == -1)
            return 0;
        else
            return 1;
    }
}

#endif