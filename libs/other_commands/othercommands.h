#ifndef OTHER_COMMANDS
#define OTHER_COMMANDS

#include "../../utils/global.h"
#include "../../utils/signalhandlers.h"
#include "./backgroundprocesses.h"
#include "./foregroundprocesses.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

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