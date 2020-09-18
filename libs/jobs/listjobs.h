#ifndef LISTJOBS
#define LISTJOBS

#include "../../utils/global.h"
#include <stdio.h>
#include <unistd.h>

void listjobs()
{
    for (int a = 0; a < commandCnt; a++)
    {
        if (getpgid(processesID[a]) >= 0)
            printf("[%d] Running %s [%d]\n", a + 1, processesName[a], processesID[a]);
        else
            printf("[%d] Stopped %s [%d]\n", a + 1, processesName[a], processesID[a]);
    }
}

#endif