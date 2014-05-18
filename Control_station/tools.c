//by Sébastien Malissard

#include "tools.h"

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

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


//tools for write on keyboard

void wait(void){
    printf("Appuyer sur entrée pour continuer\n");
    while(getchar()!='\n');
    }

int enterString(int nb, char *ret){ //nb is the maximun of char
    char s[SIZE_CHAR_KEYBOARD] = {0};
    int i;

    if(nb > SIZE_CHAR_KEYBOARD - 2){ //example of string : "5\n\0"
        return -1;
        }

    fgets(s, SIZE_CHAR_KEYBOARD, stdin);

    if((strlen(s) - 1) > nb){
        printf("Erreur de syntaxe0\n\n");
        wait();
        return -1;
        }

    for(i = 0 ; i < (strlen(s) - 1) ; i++){
        ret[i] = s[i]; 
        }

    return i;
    }

int enterNum(void){ //only positive value
    char s[SIZE_CHAR_KEYBOARD] = {'\0'};
    int i;

    enterString(SIZE_CHAR_KEYBOARD - 2, s);

    for(i = 0 ; i < SIZE_CHAR_KEYBOARD ; i++){
        if( s[i] == '\0'){ //end of the string
            break;
            }
        if( s[i] < '0' || s[i] > '9' ){
            printf("Erreur de syntax\n");
            wait();
            return -1;
            }
        }

    return atoi(s);
    }

int enterIP(char *ip){
    char s[SIZE_CHAR_KEYBOARD] = {0};
    int i, j = 0, k = 0;

    enterString(SIZE_CHAR_KEYBOARD - 2, s);

    for(i = 0 ; i < SIZE_CHAR_KEYBOARD; i++){
        if( s[i] < '0' || s[i] > '9'){
            if(s[i] != '\0' && s[i] != '.'){
                printf("Erreur de syntax1 : %d\n", i);
                wait();
                return -1;
                }
            }
        if( s[i] >= '0' && s[i] <= '9' ){ 
            j++;
            ip[i] = s[i] ;
            }
        if( s[i] == '.'){
            if(j > 3 || j == 0){
                printf("Erreur de syntax2, %d\n",i);
                wait();
                return -1;
                }
            j = 0;
            k++;
            ip[i] = s[i];
            }
        if ( k > 3){
            printf("Erreur de syntax3 : %d\n",i);
            wait();
            return -1;
            }
        if ( s[i] == '\0'){
            if(k != 3 || j == 0 || j > 3){
                printf("Erreur de syntax4, %d\n",i);
                wait();
                return -1;
                }
            break;
            }
        }

    return 1;
    }

