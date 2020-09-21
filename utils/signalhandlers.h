#ifndef SIGNAL_HANDLERS
#define SIGNAL_HANDLERS

#include "./global.h"
#include "./prompt.h"
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void processEnd()
{
    for (int a = 0; a < commandCnt; a++)
    {
        int status;
        if (waitpid(processesID[a], &status, WNOHANG | WUNTRACED) > 0)
        {
            printf("[%d] %s with pid %d exited ", a + 1, processesName[a], processesID[a]);
            if (WIFEXITED(status) > 0)
                printf("normally");
            else if (WIFEXITED(status))
                printf("with status %d", status);
            else
                printf("abnormally");
            printf("\n");
        }
    }
}

void handleSigint()
{
    if (curForegroundProcess != shellPID)
    {
        kill(curForegroundProcess, SIGINT);
        curForegroundProcess = shellPID;
        interrupted = 1;
    }
    else
    {
        printf("\n");
        shellPrompt();
    }
}

void handleSigtstp()
{
    if (curForegroundProcess == shellPID)
        return;
    processesID[commandCnt] = curForegroundProcess;
    strcpy(processesName[commandCnt], args[0]);
    kill(curForegroundProcess, SIGTSTP);
    printf("[%d] %d\n", ++commandCnt, curForegroundProcess);
    curForegroundProcess = shellPID;
    interrupted = 1;
}

#endif