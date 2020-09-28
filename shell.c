#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include "./libs/builtins/builtin.h"
#include "./utils/getinput.h"
#include "./utils/global.h"
#include "./utils/checkpipes.h"
#include "./libs/ls/ls.h"
#include "./libs/pinfo/pinfo.h"
#include "./libs/other_commands/othercommands.h"
#include "./libs/history/history.h"
#include "./libs/nightswatch/nightswatch.h"
#include "./utils/prompt.h"
#include "./libs/envupdate/envupdate.h"
#include "./libs/jobs/updatejobs.h"

char curPath[MAX_PATH_LENGTH];        // Current path of shell
char prompt[MAX_SHELL_PROMPT_LENGTH]; // Prompt

void lookup()
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
    {
        printf("%s\n", getenv(args[0] + 1));
    }
    else
        commandStatus = otherCommands();
}

void execute_pipe(int in, int out)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        args[numargs] = NULL;
        if (in != 0)
        {
            dup2(in, 0);
            close(in);
        }
        if (out != 1)
        {
            dup2(out, 1);
            close(out);
        }
        lookup();
        exit(0);
    }
}

void lookup_pipes()
{
    char **tmpargs = args;
    int totalargs = numargs;
    numargs = 1;
    int fd[2];
    int in = 0;
    for (int a = 1; a < totalargs; a++)
    {
        if (strcmp(tmpargs[a], "|") == 0)
        {
            pipe(fd);
            execute_pipe(in, fd[1]);
            close(fd[1]);
            in = fd[0];
            args += numargs;
            args++;
            numargs = 1;
            a++;
        }
        else
            numargs++;
        if (!commandStatus)
        {
            numargs = totalargs;
            args = tmpargs;
            return;
        }
    }
    execute_pipe(in, 1);
    numargs = totalargs;
    args = tmpargs;
}

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