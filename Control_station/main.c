//gcc -pthread main_IHM.c -o test


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "menu.h"
#include "type.h"
#include "param.h"

void *main_ihm(void* arg){

    int shut_down = 0;
    int temp, select = 0;
    char c;

    init();
    wait();

    while(!shut_down){

        menu(select);
        printf("\n");

        if(enterChar(1, &c) == -1){
            continue;
            }

        switch(c){
            case 's' :
                if( (temp = selectRobot()) != -1){
                    select = temp;
                    }
                wait();
                break;
            case 'a' :
                addRobot();
                wait();
                break;
            case 'q' :
                printf("Bye...\n");
                shut_down = 1;
                break;
            case 'i' :
                break;
            case 'n' :
                break;
            case 'r' :
                break;
            default :
                printf("Choix invalide\n");
                break;
            }
        }

    pthread_exit(NULL);
    }

int main(void){
    pthread_t th1, th2;

    if(pthread_create(&th1, NULL, main_ihm, NULL) == -1){
    	perror("pthread_create");
        return -1;
        }

    if (pthread_join(th1, NULL)) {
	    perror("pthread_join");
	    return -1;
        }


    return 1;
    }
