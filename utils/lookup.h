#ifndef LOOKUP
#define LOOKUP

#include <string.h>

#include "getinput.h"
#include "global.h"
#include "prompt.h"
#include "checkpipes.h"
#include "../libs/ls/ls.h"
#include "../libs/pinfo/pinfo.h"
#include "../libs/builtins/builtin.h"
#include "../libs/other_commands/othercommands.h"
#include "../libs/history/history.h"
#include "../libs/nightswatch/nightswatch.h"
#include "../libs/envupdate/envupdate.h"
#include "../libs/jobs/updatejobs.h"

void final_lookup()
{
    if (strcmp(args[0], "quit") == 0)
    {
        addCommand();
        _exit(0);
    }
    else if (strcmp(args[0], "pwd") == 0)
        commandStatus = pwd();
    else if (strcmp(args[0], "cd") == 0)
        commandStatus = cd();
    else if (strcmp(args[0], "echo") == 0)
        commandStatus = echo();
    else if (strcmp(args[0], "ls") == 0)
        commandStatus = ls();
    else if (strcmp(args[0], "pinfo") == 0)
        commandStatus = pinfo();
    else if (strcmp(args[0], "history") == 0)
        commandStatus = history();
    else if (strcmp(args[0], "nightswatch") == 0)
        commandStatus = nightswatch();
    else if (strcmp(args[0], "setenv") == 0 || strcmp(args[0], "unsetenv") == 0)
        commandStatus = envupdate();
    else if (strcmp(args[0], "jobs") == 0 || strcmp(args[0], "kjob") == 0 || strcmp(args[0], "fg") == 0 || strcmp(args[0], "overkill") == 0 || strcmp(args[0], "bg") == 0)
        commandStatus = updatejobs();
    else if (args[0][0] == '$')
        printf("%s\n", getenv(args[0] + 1));
    else
        commandStatus = otherCommands();
}

void lookup()
{
    char **argtemp = args;
    int totalargs = numargs;
    numargs = 0;
    int done = 0;
    for (int a = 0; a < totalargs; a++)
    {
        if (strcmp(argtemp[a], "$") == 0)
        {
            final_lookup();
            if (commandCnt == 0)
            {
                args = argtemp;
                numargs = totalargs;
                return;
            }
            else
            {
                args = argtemp + a + 1;
                numargs = 0;
            }
        }
        else if (strcmp(argtemp[a], "@") == 0)
        {
            final_lookup();
            if (commandCnt == 1)
            {
                args = argtemp;
                numargs = totalargs;
                return;
            }
            else
            {
                args = argtemp + a + 1;
                numargs = 0;
            }
        }
        else
            numargs++;
    }
    final_lookup();
    numargs = totalargs;
    args = argtemp;
}

#endif