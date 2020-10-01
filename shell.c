#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include "./utils/getinput.h"
#include "./utils/global.h"
#include "./utils/checkpipes.h"
#include "./utils/lookup.h"
#include "./utils/prompt.h"
#include "./utils/piping.h"
#include "./libs/ls/ls.h"
#include "./libs/builtins/builtin.h"
#include "./libs/pinfo/pinfo.h"
#include "./libs/other_commands/othercommands.h"
#include "./libs/history/history.h"
#include "./libs/nightswatch/nightswatch.h"
#include "./libs/envupdate/envupdate.h"
#include "./libs/jobs/updatejobs.h"

char curPath[MAX_PATH_LENGTH];        // Current path of shell
char prompt[MAX_SHELL_PROMPT_LENGTH]; // Prompt

int main(int agrc, char *agrv[])
{
    exit_status[0] = ':';
    exit_status[1] = 39;
    exit_status[2] = ')';
    exit_status[3] = '\0';
    signal(SIGINT, handleSigint);
    signal(SIGTSTP, handleSigtstp);
    if (getcwd(shellPath, sizeof(shellPath)) == NULL)
    {
        perror("Shell path error: ");
        return 1;
    }
    strcpy(lwd, shellPath);
    curForegroundProcess = shellPID = (int)getpid();
    multiargs = (char **)malloc(MAX_INPUT * sizeof(char *));
    while (1)
    {
        shellPrompt();
        inp = (char *)malloc(INPUT_LENGTH * sizeof(char));
        strcpy(inp, readLine());
        int len = strlen(inp);
        inp[len - 1] = '\0';
        len = strlen(inp);
        if (len == 0)
        {
            commandStatus = 1;
            continue;
        }
        separateMultiArgs(inp);
        for (int a = 0; a < numCommands; a++)
        {
            if (strlen(multiargs[a]) == 0)
                continue;
            args = separateInput(multiargs[a]);
            if (numargs == 0)
                continue;
            int todo = parseInputFiles();
            parseOutputFiles();
            if (!todo)
                continue;
            if (checkpipes())
                lookup_pipes();
            else
                lookup();
            addCommand();
            free(args);
            if (changedStdOut)
                resetOutputfile();
            if (changedStdIn)
                resetInputFile();
        }
        free(inp);
    }
    return 0;
}
