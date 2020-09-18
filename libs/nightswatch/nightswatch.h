#ifndef NIGHTSWATCH
#define NIGHTSWATCH

#include "../../utils/global.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void getthirdline(int st)
{
    int fd = open("/proc/interrupts", O_RDONLY);
    int idx = 0;
    char threadline[MAX_FILE_INFO];
    read(fd, &threadline[idx++], sizeof(char));
    while (threadline[idx - 1] != '\n')
        read(fd, &threadline[idx++], sizeof(char));
    idx = 0;
    read(fd, &threadline[idx++], sizeof(char));
    while (threadline[idx - 1] != '\n')
        read(fd, &threadline[idx++], sizeof(char));

    idx = 0;
    read(fd, &threadline[idx++], sizeof(char));
    while (threadline[idx - 1] != '\n')
        read(fd, &threadline[idx++], sizeof(char));
    for (int a = st; a < idx; a++)
    {
        if (threadline[a] != ' ' && ((int)threadline[a] < 48 || (int)threadline[a] > 57))
            break;
        printf("%c", threadline[a]);
    }
    printf("\n");
    close(fd);
}

void getinterrupts(int intervals)
{
    pid_t flg = fork();
    if (flg < 0)
        printf("%s: unable to fork\n", args[0]);
    else if (flg == 0)
    {
        int fd = open("/proc/interrupts", O_RDONLY);
        char threadline[MAX_FILE_INFO];
        int idx = 0;
        read(fd, &threadline[idx++], sizeof(char));
        while (threadline[idx - 1] != '\n')
            read(fd, &threadline[idx++], sizeof(char));
        int st = 0;
        while (threadline[st] == ' ')
            st++;
        for (int a = st; a < idx; a++)
            printf("%c", threadline[a]);
        close(fd);
        for (;;)
        {
            getthirdline(st);
            sleep(intervals);
        }
        exit(0);
    }
    else
    {
        char *inp = (char *)malloc(MAX_INPUT * sizeof(char));
        while (1)
        {
            scanf("%s", inp);
            if (strcmp(inp, "q") == 0)
            {
                kill(flg, SIGTERM);
                break;
            }
        }
    }
}

void getnewborns(int intervals)
{
    pid_t flg = fork();
    if (flg < 0)
        printf("%s: unable to fork\n", args[0]);
    else if (flg == 0)
    {
        for (;;)
        {
            int fd = open("/proc/loadavg", O_RDONLY);
            char rd;
            int cnt = 0;
            while (cnt < 4)
            {
                read(fd, &rd, sizeof(char));
                if (rd == ' ')
                    cnt++;
            }
            while (read(fd, &rd, sizeof(char)))
                printf("%c", rd);
            close(fd);
            sleep(intervals);
        }
        exit(0);
    }
    else
    {
        char *inp = (char *)malloc(MAX_INPUT * sizeof(char));
        while (1)
        {
            scanf("%s", inp);
            if (strcmp(inp, "q") == 0)
            {
                kill(flg, SIGTERM);
                break;
            }
        }
    }
}

void nightswatch()
{
    if (numargs != 4)
    {
        printf("nightswatch: Too few arguments\n");
        return;
    }
    int nFlag = 0;
    int intervals = -1;
    if (strcmp(args[1], "-n") == 0)
    {
        nFlag = 1;
        intervals = 0;
        int len = strlen(args[2]);
        for (int a = 0; a < len; a++)
            if (args[2][a] < 48 || args[2][a] > 57)
            {
                printf("nightswatch: Invalid arguments\n");
                return;
            }
        for (int a = 0; a < len; a++)
        {
            intervals *= 10;
            intervals += (int)((int)args[2][a] - 48);
        }
    }
    if (strcmp(args[3], "interrupt") == 0)
        getinterrupts(intervals);
    else if (strcmp(args[3], "newborn") == 0)
        getnewborns(intervals);
    else
        printf("nightswatch: Invaid option\n");
    return;
}

#endif