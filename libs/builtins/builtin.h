#ifndef BUILTIN_FUNCTIONS
#define BUILTIN_FUNCTIONS

#include "../../utils/global.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

void pwd()
{
    if (numargs > 1)
    {
        printf("pwd: Too many arguments!\n");
        return;
    }
    char curPath[MAX_PATH_LENGTH];
    if (getcwd(curPath, sizeof(curPath)) != NULL)
        printf("%s\n", curPath);
    else
        perror("pwd");
}

void makePath(char destination[])
{
    int len_args = strlen(args[1]);
    if (len_args == 0)
        return;
    int len = strlen(destination);
    if (len_args == 2 && args[1][0] == '.' && args[1][1] == '.')
    {
        if (strcmp(destination, "/") == 0)
            return;
        destination[len - 1] = '\0';
        len--;
        while (destination[len - 1] != '/')
        {
            destination[len - 1] = '\0';
            len--;
        }
        return;
    }
    if (len_args >= 3 && args[1][0] == '.' && args[1][1] == '.' && args[1][2] == '/')
    {
        if (strcmp(destination, "/"))

        {
            destination[len - 1] = '\0';
            len--;
            while (destination[len - 1] != '/')
            {
                destination[len - 1] = '\0';
                len--;
            }
        }
        for (int a = 3; a < len_args; a++)
            args[1][a - 3] = args[1][a];
        for (int a = len_args - 3; a < len_args; a++)
            args[1][a] = '\0';
        if (len_args > 3)
            makePath(destination);
        return;
    }
    if (len_args == 1 && args[1][0] == '.')
        return;
    if (len_args >= 2 && args[1][0] == '.' && args[1][1] == '/')
    {
        for (int a = 2; a < len_args; a++)
            args[1][a - 2] = args[1][a];
        args[1][len_args - 1] = args[1][len_args - 2] = '\0';
        makePath(destination);
        return;
    }
    char here[MAX_PATH_LENGTH];
    int idx = 0;
    for (int a = 0; a < MAX_PATH_LENGTH; a++)
        here[a] = '\0';
    while (idx < len_args && args[1][idx] != '/')
    {
        here[idx] = args[1][idx];
        idx++;
    }
    strcat(destination, here);
    strcat(destination, "/");
    idx++;
    if (idx >= len_args)
        return;
    for (int a = idx; a < len_args; a++)
        args[1][a - idx] = args[1][a];
    for (int a = len_args - idx; a < len_args; a++)
        args[1][a] = '\0';
    makePath(destination);
}

void cd()
{
    if (numargs > 2)
    {
        printf("cd: Too many arguments!\n");
        return;
    }
    if (numargs == 1)
    {
        if (getcwd(lwd, sizeof(lwd)) == NULL)
        {
            perror("Path error");
            _exit(1);
        }
        chdir(shellPath);
        return;
    }
    if (args[1][0] == '-')
    {
        chdir(lwd);
        printf("%s\n", lwd);
        return;
    }
    if (args[1][0] == '/')
    {
        DIR *dir = opendir(args[1]);
        if (dir == NULL)
        {
            perror("cd");
            return;
        }
        if (getcwd(lwd, sizeof(lwd)) == NULL)
        {
            perror("Path error");
            _exit(1);
        }
        chdir(args[1]);
        return;
    }
    char destination[MAX_PATH_LENGTH];
    for (int a = 0; a < MAX_PATH_LENGTH; a++)
        destination[a] = '\0';
    if ((strlen(args[1]) == 1 && args[1][0] == '~') || (strlen(args[1]) > 1 && args[1][0] == '~' && args[1][1] == '/'))
    {
        strcpy(destination, shellPath);
        if (strlen(args[1]) == 1)
        {
            if (getcwd(lwd, sizeof(lwd)) == NULL)
            {
                perror("Path error");
                _exit(1);
            }
            chdir(shellPath);
            return;
        }
        args[1]++;
        args[1]++;
    }
    else if (getcwd(destination, sizeof(destination)) == NULL)
    {
        perror("Path error");
        return;
    }
    int len = strlen(destination);
    if (destination[len - 1] != '/')
    {
        destination[len] = '/';
        len++;
    }
    makePath(destination);
    DIR *dir = opendir(destination);
    if (dir == NULL)
        perror("cd");
    else
    {
        if (getcwd(lwd, sizeof(lwd)) == NULL)
        {
            perror("Path error");
            _exit(1);
        }
        chdir(destination);
    }
}

void echo()
{
    for (int a = 1; a < numargs; a++)
        printf("%s ", args[a]);
    printf("\n");
}
#endif