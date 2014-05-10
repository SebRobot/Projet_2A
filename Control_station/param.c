//by Sébastien Malissard

#include "param.h"

#include <stdio.h>
#include <pthread.h>

#include "type.h"

listRobot robot[NB_ROBOT];

pthread_mutex_t mutexRWrobot = PTHREAD_MUTEX_INITIALIZER;

int rwRobot(eRW rw, listRobot *p){
    
    if(p == NULL){
        return -1;
        }

   if( pthread_mutex_lock(&mutexRWrobot) != 0){
        printf("Error mutex lock\n");        
        }

    //TODO vérifier si les donées sont valide
    switch(rw){
        case READ:
            *p = robot[p->num]; 
            break;
        case WRITE:
            robot[p->num] = *p;
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


