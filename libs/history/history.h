#ifndef HISTORY
#define HISTORY

#include "../../utils/global.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int getSize(const char *path)
{
    struct stat *stats;
    stats = malloc(sizeof(struct stat));
    stat(path, stats);
    return stats->st_size;
}

int history()
{
    if (numargs > 2)
    {
        printf("history: too many arguments\n");
        return 0;
    }
    char tmp[MAX_PATH_LENGTH];
    strcpy(tmp, shellPath);
    strcat(tmp, historyFilePath);
    int fileReader = open(tmp, O_RDONLY);
    char historyReader[20 * INPUT_LENGTH];
    for (int a = 0; a < 20 * INPUT_LENGTH; a++)
        historyReader[a] = '\0';
    int len = getSize(tmp);
    read(fileReader, historyReader, len * sizeof(char));
    close(fileReader);
    if (numargs == 1)
    {
        args[numargs] = (char *)malloc(5 * sizeof(char));
        args[numargs][0] = '1';
        args[numargs][1] = '0';
        args[numargs][2] = '\0';
        numargs++;
    }
    len = strlen(args[1]);
    for (int a = 0; a < len; a++)
        if (args[1][a] < 48 || args[1][a] > 57)
        {
            printf("history: invalid arguments\n");
            return 0;
        }
    int cnt = 0;
    len = strlen(historyReader);
    for (int a = 0; a < len; a++)
        if (historyReader[a] == '\n')
            cnt++;
    int numPrint = cnt;
    int x = 0;
    len = strlen(args[1]);
    for (int a = 0; a < len; a++)
    {
        x *= 10;
        x += ((int)args[1][a] - 48);
    }
    if (x < numPrint)
        numPrint = x;
    int toSkip = cnt - numPrint;
    int idx = 0;
    if (toSkip != 0)
    {
        int skipped = 0;
        len = strlen(historyReader);
        for (; skipped < toSkip && idx < len; idx++)
            if (historyReader[idx] == '\n')
                skipped++;
    }
    len = strlen(historyReader);
    int hisNumber = toSkip + 1;
    printf("%d ", hisNumber);
    while (idx < len)
    {
        printf("%c", historyReader[idx]);
        if (historyReader[idx] == '\n' && idx < len - 1)
            printf("%d ", ++hisNumber);
        idx++;
    }
    return 1;
}

void addCommand()
{
    char tmp[MAX_PATH_LENGTH];
    strcpy(tmp, shellPath);
    strcat(tmp, historyFilePath);
    int fileReader = open(tmp, O_RDONLY);
    char historyReader[20 * INPUT_LENGTH];
    for (int a = 0; a < 20 * INPUT_LENGTH; a++)
        historyReader[a] = '\0';
    int len = getSize(tmp);
    read(fileReader, historyReader, len * sizeof(char));
    int cnt = 0;
    for (int a = 0; a < len; a++)
        if (historyReader[a] == '\n')
            cnt++;
    if (cnt >= 20)
    {
        int idx = 0;
        while (historyReader[idx] != '\n')
            idx++;
        idx++;
        int a;
        for (a = 0; idx < len; idx++, a++)
            historyReader[a] = historyReader[idx];
        while (a < len)
            historyReader[a++] = '\0';
    }
    for (int i = 0; i < numargs; i++)
    {
        int x = strlen(args[i]);
        len = strlen(historyReader);
        for (int j = 0; j < x; j++)
            historyReader[len + j] = args[i][j];
        historyReader[len + x] = ' ';
    }
    len = strlen(historyReader);
    historyReader[len++] = '\n';
    int fileWriter = open(tmp, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    write(fileWriter, historyReader, len * sizeof(char));
    close(fileReader);
    close(fileWriter);
}

#endif