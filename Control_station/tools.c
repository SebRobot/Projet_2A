//by Sébastien Malissard

#include "tools.h"

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "param.h"
#include "type.h"


pthread_mutex_t mutexRWrobot = PTHREAD_MUTEX_INITIALIZER;


void printConsole(char *msg){
    if(pipeConsole != 0){
        write(pipeConsole, msg, SIZE_MESSAGE_CONSOLE);
        }
    }


int rwRobot(eRW rw, int num, infoRobot *rob){ //TODO dvp pour changer que certain parametre
    /*if(p == NULL){
        return -1;
        }
*/
   if( pthread_mutex_lock(&mutexRWrobot) != 0){
        printf("Error mutex lock\n");
        return -1;       
        }


/*    printf("=> idx%i (before loc%i|ip%s) .. ", num, rob->loc, rob->ip);*/

    //TODO vérifier si les donées sont valide
    switch(rw){
        case READ:
            memcpy(rob, &robot[num], sizeof(infoRobot)); 
            break;
        case WRITE:
            memcpy(&robot[num], rob, sizeof(infoRobot)); 
            break;
        default:
            return -1;
            break;
        }

/*    printf("loc%i|ip%s\n", rob->loc, rob->ip);*/

   if( pthread_mutex_unlock(&mutexRWrobot) != 0){
        printf("Error mutex lock\n");
        return -1;
        }

    return 1;
    }


//tools for write on keyboard

void wait(void){
    printf("Appuyer sur entrée pour continuer\n");
    while(getchar()!='\n');
    }

int enterString(int nb, char *ret){ //nb is the maximun of character
    char s[SIZE_CHAR_KEYBOARD] = {'\0'};

    fgets(s, SIZE_CHAR_KEYBOARD, stdin);

    if(s[strlen(s)-1] == '\n'){
        s[strlen(s)-1] = '\0';
        }

    if(strlen(s) > nb){
        printf("Erreur de syntaxe\n");
        wait();
        return -1;
        }

    memcpy(ret, s, strlen(s));

    return nb;
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
    struct sockaddr_in serv_addr;
    char s[SIZE_CHAR_KEYBOARD] = {'\0'};

    enterString(SIZE_CHAR_KEYBOARD - 2, s);

    if(inet_pton(AF_INET, s, &serv_addr.sin_addr) <= 0){ //just check if ip is valid
        printf("Erreur adresse non valide\n");
        return -1;
        } 

    memcpy(ip, s, strlen(s));

    return 1;
    }

