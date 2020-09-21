#ifndef NW_INTERRUPTS
#define NW_INTERRUPTS

#include "../../utils/global.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>

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
        char *inp = (char *)malloc(INPUT_LENGTH * sizeof(char));
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

#endif