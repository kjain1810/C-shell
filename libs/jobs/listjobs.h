#ifndef LISTJOBS
#define LISTJOBS

#include "../../utils/global.h"
#include <stdio.h>
#include <unistd.h>

int listjobs()
{
    for (int a = 0; a < commandCnt; a++)
    {
        if (getpgid(processesID[a]) >= 0)
        {
            // printf("[%d] Running %s [%d]\n", a + 1, processesName[a], processesID[a]);
            char processFile[1024];
            strcpy(processFile, "/proc/");
            char pidinchar[10];
            int len = 0, tmp = processesID[a];
            while (tmp > 0)
            {
                pidinchar[len++] = 48 + tmp % 10;
                tmp /= 10;
            }
            for (int a = 0; a < len / 2; a++)
            {
                char tmp = pidinchar[a];
                pidinchar[a] = pidinchar[len - a - 1];
                pidinchar[len - a - 1] = tmp;
            }
            pidinchar[len] = '\0';
            strcat(processFile, pidinchar);
            strcat(processFile, "/stat");
            FILE *reader = fopen(processFile, "r");
            char line[MAX_PROCESS_INFO];
            fgets(line, MAX_PROCESS_INFO, reader);
            char ret[MAX_PROCESS_INFO];
            for (int b = 0; b < MAX_PROCESS_INFO; b++)
                ret[a] = '\0';
            len = strlen(line);
            int cnt = 0, idx;
            for (idx = 0; idx < len; idx++)
            {
                if (line[idx] == ' ')
                    cnt++;
                if (cnt == 3 - 1)
                    break;
            }
            idx++;
            for (int a = 0; line[idx + a] != ' '; a++)
                ret[a] = line[idx + a];
            ret[1] = '\0';
            if (strcmp(ret, "T") == 0)
                printf("[%d] Stopped %s [%d]\n", a + 1, processesName[a], processesID[a]);
            else
                printf("[%d] Running %s [%d]\n", a + 1, processesName[a], processesID[a]);
        }
        else
            printf("[%d] Stopped %s [%d]\n", a + 1, processesName[a], processesID[a]);
    }
    return 1;
}

#endif