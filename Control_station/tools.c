//by Sébastien Malissard

#include "tools.h"

#include <stdio.h>
#include <pthread.h>

#include "param.h"
#include "type.h"


pthread_mutex_t mutexRWrobot = PTHREAD_MUTEX_INITIALIZER;


void printConsole(char *msg){
    write(pipeConsole, msg, SIZE_MESSAGE_CONSOLE);
    }


int rwRobot(eRW rw, int num, infoRobot *p){ //TODO dvp pour changer que certain parametre
    if(p == NULL){
        return -1;
        }

   if( pthread_mutex_lock(&mutexRWrobot) != 0){
        printf("Error mutex lock\n");        
        }

    //TODO vérifier si les donées sont valide
    switch(rw){
        case READ:
            *p = robot[num]; 
            break;
        case WRITE:
            robot[num] = *p;
            break;
        default:
            return -1;
            break;
        }

   if( pthread_mutex_unlock(&mutexRWrobot) != 0){
        printf("Error mutex lock\n");        
        }

    return 1;
    }

