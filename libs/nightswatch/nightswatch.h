#ifndef NIGHTSWATCH
#define NIGHTSWATCH

#include "../../utils/global.h"
#include "./nw_interrupts.h"
#include "./nw_newborn.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void nightswatch()
{
    if (numargs != 4)
    {
        printf("nightswatch: Too few arguments\n");
        return;
    }
    int nFlag = 0;
    int intervals = -1;
    if (strcmp(args[1], "-n") == 0)
    {
        nFlag = 1;
        intervals = 0;
        int len = strlen(args[2]);
        for (int a = 0; a < len; a++)
            if (args[2][a] < 48 || args[2][a] > 57)
            {
                printf("nightswatch: Invalid arguments\n");
                return;
            }
        for (int a = 0; a < len; a++)
        {
            intervals *= 10;
            intervals += (int)((int)args[2][a] - 48);
        }
    }
    if (strcmp(args[3], "interrupt") == 0)
        getinterrupts(intervals);
    else if (strcmp(args[3], "newborn") == 0)
        getnewborns(intervals);
    else
        printf("nightswatch: Invaid option\n");
    return;
}

#endif