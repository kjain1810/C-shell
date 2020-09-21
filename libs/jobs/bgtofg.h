#ifndef BGTOFG
#define BGTOFG

#include "../../utils/global.h"
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

void bringtoforeground()
{
    if (numargs != 2)
    {
        printf("%s: invalid number of arguments\n", args[0]);
        return;
    }
    int len_arg1 = strlen(args[1]);
    for (int a = 0; a < len_arg1; a++)
        if (args[1][a] < 48 || args[1][a] > 57)
        {
            printf("%s: invalid argument\n", args[0]);
            return;
        }
    int arg1 = 0;
    for (int a = 0; a < len_arg1; a++)
    {
        arg1 *= 10;
        arg1 += (int)((int)args[1][a] - 48);
    }
    if (arg1 > commandCnt || arg1 == 0)
    {
        printf("%s: no such job\n", args[0]);
        exit_status[2] = '(';
        return;
    }
    kill(processesID[arg1 - 1], SIGCONT);
    tcsetpgrp(0, getpgid(processesID[arg1 - 1]));
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    int x = waitpid(getpgid(processesID[arg1 - 1]), NULL, 0);
    tcsetpgrp(0, getpgid(shellPID));
    signal(SIGTTOU, SIG_DFL);
    signal(SIGTTIN, SIG_DFL);
    if (x == -1)
        exit_status[2] = '(';
    else
        exit_status[2] = ')';
}

#endif