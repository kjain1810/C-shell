#ifndef UPDATEJOBS
#define UPDATEJOBS

#include "../../utils/global.h"
#include "./listjobs.h"
#include "./sendsignal.h"
#include "./bgtofg.h"
#include "./killall.h"
#include "./startbg.h"
#include <string.h>

int updatejobs()
{
    if (strcmp(args[0], "jobs") == 0)
        return listjobs();
    else if (strcmp(args[0], "kjob") == 0)
        return sendsignal();
    else if (strcmp(args[0], "fg") == 0)
        return bringtoforeground();
    else if (strcmp(args[0], "overkill") == 0)
        return killall();
    else if (strcmp(args[0], "bg") == 0)
        return startbg();
    printf("Why am I here in libs/jobs/updatejobs.h?\n");
    return 0;
}

#endif