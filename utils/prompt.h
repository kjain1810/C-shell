#ifndef PROMPT_FILE
#define PROMPT_FILE
#include <sys/utsname.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include "global.h"

void shellPrompt()
{
    struct utsname unameData;
    uname(&unameData);
    curPath[1023] = '\0';
    if (getcwd(curPath, sizeof(curPath)) != NULL)
    {
        if (strcmp(shellPath, curPath) == 0)
            strcpy(curPath, "~");
        else if (checkShellHome(curPath))
        {
            int homeLen = strlen(shellPath);
            int curLen = strlen(curPath);
            curPath[0] = '~';
            for (int a = homeLen; a < curLen; a++)
                curPath[a - homeLen + 1] = curPath[a];
            curPath[curLen - homeLen + 1] = '\0';
        }
    }
    else
    {
        perror("Error in getting current working directory: ");
        exit(0);
    }
    struct passwd *p = getpwuid(getuid());
    char *userName = p->pw_name;
    char *systemName = unameData.nodename;
    sprintf(prompt, "<%s@%s:%s> ", userName, systemName, curPath);
    printf("\033[0;32m%s\033[0m", prompt);
    fflush(stdout);
}

#endif