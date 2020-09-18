#ifndef PINFO
#define PINFO

#include "../../utils/global.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

int pid_exist()
{
    char processFile[MAX_PATH_LENGTH];
    for (int a = 0; a < MAX_PATH_LENGTH; a++)
        processFile[a] = '\0';
    strcpy(processFile, "/proc/");
    strcat(processFile, args[1]);
    struct stat stats;
    int tmp = stat(processFile, &stats);
    if (tmp < 0 && errno == ENOENT)
        return 0;
    return 1;
}

void get_nth_word(char *line, int n)
{
    char ret[MAX_PROCESS_INFO];
    for (int a = 0; a < MAX_PROCESS_INFO; a++)
        ret[a] = '\0';
    int len = strlen(line);
    int cnt = 0, idx;
    for (idx = 0; idx < len; idx++)
    {
        if (line[idx] == ' ')
            cnt++;
        if (cnt == n - 1)
            break;
    }
    idx++;
    for (int a = 0; line[idx + a] != ' '; a++)
        ret[a] = line[idx + a];
    printf("%s\n", ret);
}

void pinfo()
{
    if (numargs == 1)
        printf("pid -- %d\n", shellPID);
    else if (numargs > 2)
    {
        printf("pinfo: Too many arguments.\n");
        return;
    }
    else
    {
        if (pid_exist())
            printf("pid -- %s\n", args[1]);
        else
        {
            printf("No process with pid %s found right now.\n", args[1]);
            return;
        }
    }
    char *status = (char *)malloc(MAX_FILE_INFO * sizeof(char));
    status[0] = status[1] = status[2] = '\0';
    char *memoryUsage = (char *)malloc(MAX_FILE_INFO * sizeof(char));
    char processFile[MAX_PATH_LENGTH];
    strcpy(processFile, "/proc/");
    if (numargs > 1)
        strcat(processFile, args[1]);
    else
    {
        char here[20];
        for (int a = 0; a < 20; a++)
            here[a] = '\0';
        int tmp = shellPID;
        int len = 0;
        while (tmp > 0)
        {
            int dig = tmp % 10;
            tmp /= 10;
            here[len] = 48 + dig;
            len++;
        }
        for (int a = 0; a < len / 2; a++)
        {
            char tmp = here[a];
            here[a] = here[len - 1 - a];
            here[len - 1 - a] = tmp;
        }
        strcat(processFile, here);
    }
    strcat(processFile, "/stat");
    FILE *reader = fopen(processFile, "r");
    char line[MAX_PROCESS_INFO];
    fgets(line, MAX_PROCESS_INFO, reader);
    printf("Process Status -- ");
    get_nth_word(line, 3);
    printf("memory -- ");
    get_nth_word(line, 23);
    int len = strlen(processFile);
    for (int a = 0; a < 4; a++)
    {
        processFile[len - 1] = '\0';
        len--;
    }
    strcat(processFile, "exe");
    len = strlen(line);
    for (int a = 0; a < len; a++)
        line[a] = '\0';
    readlink(processFile, line, MAX_PROCESS_INFO);
    if (checkShellHome(line))
    {
        line[0] = '~';
        int len_SP = strlen(shellPath);
        int len_line = strlen(line);
        for (int a = len_SP; a < len_line; a++)
            line[a - len_SP + 1] = line[a];
        for (int a = len_line - len_SP + 1; a < len_line; a++)
            line[a] = '\0';
    }
    printf("Executable Path -- %s\n", line);
}

#endif PINFO