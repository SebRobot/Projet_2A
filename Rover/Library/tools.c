



#include <math.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <unistd.h>

#include "tools.h"


//***Times function***//
    unsigned int microsec(){
        struct timeval clock;
        gettimeofday(&clock,NULL);
        return clock.tv_usec + clock.tv_sec*1000000;
    }
    unsigned int millisec(){
        struct timeval clock;
        gettimeofday(&clock,NULL);
        return clock.tv_usec/1000 + clock.tv_sec*1000;
    }
