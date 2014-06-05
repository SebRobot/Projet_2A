



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
    
    
char* dspl_eTypeMsg(eTypeMsg eType){
	switch(eType){
		case CMD:  return "CMD";     break;
		case INFO: return "INFO";    break;
		default:   return "UNKNOWN"; break;
	}
}


char* dspl_eTypeCMd(eTypeCmd eType){
	switch(eType){
		case TRAJ:  return "TRAJ";    break;
		case STATE: return "STATE";   break;
		case POS:   return "POS";     break;
		default:   return "UNKNOWN";  break;
	}
}


char* dspl_eSta(eSta eType){
	switch(eType){
		case STP:  return "STP";     break;
		case MVT: return "MVT";      break;
		default:   return "UNKNOWN"; break;
	}
}
