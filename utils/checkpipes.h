#ifndef CHECK_PIPES
#define CHECK_PIPES

#include "./global.h"
#include <string.h>

int checkpipes()
{
    for (int a = 0; a < numargs; a++)
        if (strcmp(args[a], "|") == 0)
            return 1;
    return 0;
}

#endif