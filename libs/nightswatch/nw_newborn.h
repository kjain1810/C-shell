#ifndef NW_NEWBORN
#define NW_NEWBORN

#include "../../utils/global.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int getnewborns(int intervals)
{
    pid_t flg = fork();
    if (flg < 0)
    {
        printf("%s: unable to fork\n", args[0]);
        return 0;
    }
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
        return 1;
    }
}

#endif