#ifndef ENVUPDATE
#define ENVUPDATE

#include "../../utils/global.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int envupdate()
{
    if (strcmp(args[0], "setenv") == 0)
    {
        if (numargs == 1 || numargs > 3)
        {
            printf("%s: invalid number of arguments\n", args[0]);
            return 0;
        }
        int x = 1;
        if (numargs == 2)
            x = setenv(args[1], "", 1);
        else
            x = setenv(args[1], args[2], 1);
        if (x == -1)
        {
            perror("setenv");
            return 0;
        }
        return 1;
    }
    else
    {
        if (numargs != 2)
        {
            printf("%s: invalid number of arguments\n", args[0]);
            return 0;
        }
        int x = 1;
        x = unsetenv(args[1]);
        if (x == -1)
        {
            perror("unsetenv");
            return 0;
        }
        return 1;
    }
}

#endif