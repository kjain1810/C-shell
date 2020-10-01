#ifndef KILLALL
#define KILLALL

#include "../../utils/global.h"
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int killall()
{
    for (int a = 0; a < commandCnt; a++)
        if (getpgid(processesID[a]) >= 0)
            kill(processesID[a], 9);
    return 1;
}

#endif