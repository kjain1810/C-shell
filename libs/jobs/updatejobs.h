#ifndef UPDATEJOBS
#define UPDATEJOBS

#include "../../utils/global.h"
#include "./listjobs.h"
#include "./sendsignal.h"
#include "./bgtofg.h"
#include "./killall.h"
#include "./startbg.h"
#include <string.h>

void updatejobs()
{
    if (strcmp(args[0], "jobs") == 0)
        listjobs();
    else if (strcmp(args[0], "kjob") == 0)
        sendsignal();
    else if (strcmp(args[0], "fg") == 0)
        bringtoforeground();
    else if (strcmp(args[0], "overkill") == 0)
        killall();
    else if (strcmp(args[0], "bg") == 0)
        startbg();
    else
        printf("Why am I here in libs/jobs/updatejobs.h?\n");
}

#endif