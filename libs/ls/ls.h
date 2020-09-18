#ifndef LS
#define LS

#include "../../utils/global.h"
#include "../builtins/builtin.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <grp.h>
#include <pwd.h>

void giveFullInfo(char file[], char fileName[])
{
    struct stat stats;
    if (stat(file, &stats) < 0)
        return;
    char permissions[10];
    for (int a = 0; a < 10; a++)
        permissions[a] = '-';
    if (S_ISDIR(stats.st_mode))
        permissions[0] = 'd';
    if (stats.st_mode & S_IRUSR)
        permissions[1] = 'r';
    if (stats.st_mode & S_IWUSR)
        permissions[2] = 'w';
    if (stats.st_mode & S_IXUSR)
        permissions[3] = 'x';
    if (stats.st_mode & S_IRGRP)
        permissions[4] = 'r';
    if (stats.st_mode & S_IWGRP)
        permissions[5] = 'w';
    if (stats.st_mode & S_IXGRP)
        permissions[6] = 'x';
    if (stats.st_mode & S_IROTH)
        permissions[7] = 'r';
    if (stats.st_mode & S_IWOTH)
        permissions[8] = 'w';
    if (stats.st_mode & S_IXOTH)
        permissions[9] = 'x';
    struct group *grp = getpwuid(stats.st_uid);
    struct passwd *pwd = getgrgid(stats.st_gid);
    time_t *t = malloc(1024);
    *t = stats.st_mtime;
    struct tm tm = *localtime(t);
    char *month = ret[tm.tm_mon];
    printf("%s", permissions);
    printf("%2ld ", stats.st_nlink);
    printf("%s ", pwd->pw_name);
    printf("%s ", grp->gr_name);
    printf("%9ld ", stats.st_size);
    printf("%s ", month);
    printf("%2d ", tm.tm_mday);
    printf("%02d:", tm.tm_hour);
    printf("%02d ", tm.tm_min);
    printf("%s", fileName);
    printf("\n");
}

void ls()
{
    int flagA = 0;
    int flagL = 0;
    int cnt = 0;
    for (int a = 1; a < numargs; a++)
    {
        if (args[a][0] != '-')
            continue;
        int x = strlen(args[a]);
        if (x == 1)
            continue;
        for (int b = 1; b < x; b++)
            if (args[a][b] == 'a')
                flagA = 1;
            else if (args[a][b] == 'l')
                flagL = 1;
            else
            {
                printf("ls: invalid option -- '%c'\n", args[a][b]);
                return;
            }
        cnt++;
    }
    int num_dir = numargs - 1 - cnt;
    int shown = 1;
    if (num_dir == 0)
    {
        args[numargs] = (char *)malloc(10 * sizeof(char));
        strcpy(args[numargs], ".");
        numargs++;
    }
    for (int a = 1; a < numargs; a++)
    {
        int x = strlen(args[a]);
        if (args[a][0] == '-' && x > 1)
            continue;
        if (args[a][0] == '~')
        {
            if (strlen(args[a]) == 1)
                strcpy(args[a], shellPath);
            if (args[a][1] == '/')
            {
                char tmp[MAX_PATH_LENGTH];
                strcpy(tmp, args[a] + 1);
                strcpy(args[a], shellPath);
                strcat(args[a], tmp);
            }
        }
        DIR *dir = opendir(args[a]);
        if (dir == NULL)
        {
            perror("ls");
            continue;
        }
        if (num_dir > 1)
            printf("%s:\n", args[a]);
        if (flagL)
        {
            struct stat stats;
            stat(args[a], &stats);
            int total = 0;
            struct dirent *fileCount;
            fileCount = readdir(dir);
            while (fileCount != NULL)
            {
                char tmp[MAX_FILE_INFO];
                sprintf(tmp, "%s/%s", args[a], fileCount->d_name);
                fileCount = readdir(dir);
                struct stat stats;
                stat(tmp, &stats);
                total += stats.st_blocks;
            }
            printf("total %d\n", total / 2);
            rewinddir(dir);
        }
        struct dirent *fileHere;
        fileHere = readdir(dir);
        while (fileHere != NULL)
        {
            char tmp[MAX_FILE_INFO];
            sprintf(tmp, "%s/%s", args[a], fileHere->d_name);
            if (fileHere->d_name[0] != '.' || (fileHere->d_name[0] == '.' && flagA))
            {
                if (flagL)
                    giveFullInfo(tmp, fileHere->d_name);
                else
                    printf("%s\n", fileHere->d_name);
            }
            fileHere = readdir(dir);
        }
        if (num_dir > 1 && shown != num_dir)
            printf("\n");
        shown++;
    }
}

#endif