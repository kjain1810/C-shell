#ifndef PIPING
#define PIPING

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "lookup.h"
#include "global.h"

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
    waitpid(pid, NULL, WUNTRACED);
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
#endif