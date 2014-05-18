//gcc -pthread main.c menu.c menu.h param.c param.h type.h com.c com.h tools.c tools.h -o test -Wall
//by Sébastien Malissard


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#include <fcntl.h>



#include "menu.h"
#include "type.h"
#include "param.h"
#include "tools.h"
#include "com.h"
#include "../typeMessage.h"

int sock = 0;
pthread_mutex_t mutexSocket = PTHREAD_MUTEX_INITIALIZER;

void *mainIhm(void* arg){

    int shut_down = 0;
    int temp, select = 0, ret;
    char c;

    init();
    wait();

    while(!shut_down){

        if(c != '\n'){
            menu(select);
            }

        ret = enterString(1, &c);
        
        if(ret == -1){
            continue;
            }

        if(ret == 0){
            c = '\n';
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
            case 'l' :
                displayRobot();
                wait();
                break;
            case 'c' :
                printf("Fonctionalité pas encore disponible\n");
                wait();
                break;
            case 'q' :
                printf("Bye...\n");
                shut_down = 1;
                break;

            //if a robot is select
            case 'i' :
                printf("Fonctionalité pas encore disponible. Utiliser -l pour voir la liste des robots\n");
                break;
            case 'e' :
                sendPoint(select);
                wait();
                break;
            case 'n' :
                printf("Fonctionalité pas encore disponible\n");
                wait();
                break;
            case 'r' :
                rmRobot(select);
                wait();
                break;
            case 'd' :
                printf("Le robot n°%d a été déselectionner\n",select);
                select = 0;
                wait();
                break;
            default :
                printf("Choix invalide\n");
                break;
            }

        }

    pthread_exit(NULL);
    }

void *mainComSend(void* arg){
/*    sMsg msgOut;

    msgOut.type = CMD;
    msgOut.body.cmd.pt.x = 50;

    while(1){
        senMsg(1, &msgOut);
        }
*/
    pthread_exit(NULL);
    }


int main(void){
    pthread_t th1, th3;
	char namePipe[] = "essai.fifo";
    int id;

    id = fork();

    if(id == 0){ //child processus
       execlp("gnome-terminal", "./gnome-terminal", "-e", "/home/seb/RobotOfficiel/Projet_2A/Control_station/console essai.fifo", NULL); //FIXME
        }

    else{ //father processus

        remove(namePipe);

	    if(mkfifo(namePipe, 0644) != 0) {
		    perror("Error to create the named pipe ");
		    exit(EXIT_FAILURE);
	        }

	    if((pipeConsole = open(namePipe, O_WRONLY)) == -1){
		    perror("Error to open the named pipe ");
		    exit(EXIT_FAILURE);
	        }

        printConsole("Bienvenue sur la console de supervision des messages\n\n");


        if(pthread_create(&th1, NULL, mainIhm, NULL) == -1){
        	perror("pthread_create 1\n");
            return -1;
            }
        if(pthread_create(&th3, NULL, mainComSend, NULL) == -1){
        	perror("pthread_create 3\n");
            return -1;
            }

        if (pthread_join(th1, NULL)) {
	        perror("pthread_join\n");
	        return -1;
            }

        printConsole("exit\n");

        if (pthread_join(th3, NULL)) {
	        perror("pthread_join\n");
	        return -1;
            }

    }

    return 1;
    }
