#ifndef JOBSIGNAL
#define JOBSIGNAL

#include "../../utils/global.h"
#include <stdio.h>
#include <signal.h>

void sendsignal()
{
    if (numargs != 3)
    {
        printf("%s: invalid number of arguments\n", args[0]);
        return;
    }
    int len_arg1 = strlen(args[1]);
    int len_arg2 = strlen(args[2]);
    for (int a = 0; a < len_arg1; a++)
        if (args[1][a] > 57 || args[1][a] < 48)
        {
            printf("%s: invalid argument %s\n", args[0], args[1]);
            return;
        }
    for (int a = 0; a < len_arg2; a++)
        if (args[2][a] > 57 || args[2][a] < 48)
        {
            printf("%s: invalid argument %s\n", args[0], args[2]);
            return;
        }
    int arg1 = 0;
    for (int a = 0; a < len_arg1; a++)
    {
        arg1 *= 10;
        arg1 += (int)((int)args[1][a] - 48);
    }
    int arg2 = 0;
    for (int a = 0; a < len_arg2; a++)
    {
        arg2 *= 10;
        arg2 += (int)((int)args[2][a] - 48);
    }
    if (commandCnt < arg1 || arg1 == 0)
    {
        printf("%s: no such job\n");
        return;
    }
    kill(processesID[arg1 - 1], arg2);
}

#endif