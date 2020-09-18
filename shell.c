#include <sys/utsname.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "./libs/builtins/builtin.h"
#include "./utils/getinput.h"
#include "./utils/global.h"
#include "./libs/ls/ls.h"
#include "./libs/pinfo/pinfo.h"
#include "./libs/other_commands/othercommands.h"
#include "./libs/history/history.h"
#include "./libs/nightswatch/nightswatch.h"
#include <stdlib.h>

char curPath[MAX_PATH_LENGTH];        // Current path of shell
char prompt[MAX_SHELL_PROMPT_LENGTH]; // Prompt
char *inp;

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

int main(int agrc, char *agrv[])
{
    if (getcwd(shellPath, sizeof(shellPath)) == NULL)
    {
        perror("Shell path error: ");
        return 1;
    }
    shellPID = (int)getpid();
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
            continue;
        separateMultiArgs(inp);
        for (int a = 0; a < numCommands; a++)
        {
            if (strlen(multiargs[a]) == 0)
                continue;
            args = separateInput(multiargs[a]);
            if (numargs == 0)
                continue;
            if (strcmp(args[0], "exit") == 0)
            {
                addCommand();
                exit(0);
            }
            else if (strcmp(args[0], "pwd") == 0)
                pwd();
            else if (strcmp(args[0], "cd") == 0)
                cd();
            else if (strcmp(args[0], "echo") == 0)
                echo();
            else if (strcmp(args[0], "ls") == 0)
                ls();
            else if (strcmp(args[0], "pinfo") == 0)
                pinfo();
            else if (strcmp(args[0], "history") == 0)
                history();
            else if (strcmp(args[0], "nightswatch") == 0)
                nightswatch();
            else
                otherCommands();
            addCommand();
        }
    }
    return 0;
}