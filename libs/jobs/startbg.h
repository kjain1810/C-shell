#ifndef STARTBG
#define STARTBG

#include "../../utils/global.h"
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void startbg()
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
        return;
    }
    kill(processesID[arg1 - 1], SIGCONT);
}

#endif