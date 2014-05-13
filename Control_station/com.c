//by Sébastien Malissard

#include "com.h"
#include "param.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#include <time.h>




int initCom(const char *ip){
    pthread_t thRecv; //FIXME suprimer le thread
    struct sockaddr_in serv_addr;  
    int sock;   

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("Error : Could not create socket \n");
        return -1;
        } 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT); 

    if(inet_pton(AF_INET, ip, &serv_addr.sin_addr)<=0){
        printf("Error : inet_pton occured\n");
        return -1;
        } 

    if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
       perror("Error : Connect Failed \n");
       return -1;
       }

    if(pthread_create(&thRecv, NULL, moniRecv, (void*) &sock) == -1){
    	perror("pthread_create 2\n");
        return -1;
        }

    return sock;
    }

int senMsg(const int num, const sMsg *msg){ //num -> listRobot
    infoRobot rob;

    rob.num = num;
    rwRobot(READ, num, &rob);
    
    if(send(rob.sock, msg, sizeof(*msg), 0) < -1){
        printf("Error message not send\n");
        return -1;   
        }

    return 0;
    }

void *moniRecv(void *socket){ //monitoring the receive buffer
    int sock = *(int*)socket;
    sMsg msg;

    while(1){
        recv(sock, &msg, sizeof(msg), 0); //FIXME Acces concurent au socket ??? et aussi fragmentation du message ???

        switch(msg.type){  //TODO prendre en compte le message : mise à jour du tableau
            case CMD :
                saveHistMsg(msg.type, RECV);
                break;
            case INFO :
                
                break;
            default :
                printf("Error : type of message unknown\n");
                break;
            }
        }

    pthread_exit(NULL);
    }

void saveHistMsg(eTypeMsg type, eSendRecv sr){
    char *stType, *stSR;
    
    switch(type){
        case CMD :
            stType = "CMD";
            printf("Un message de type CMD reçue\n");
            break;
        case INFO :
            stType = "INFO";
            printf("Un message de type INFO reçue\n");
            break;
        default:
            printf("Error in switch type\n");
        }

    switch(sr){
        case SEND :
            stSR = "SEND";
            break;
        case RECV :
            stSR = "RECV";
            break;
        default:
            printf("Error in switch sr\n");
        }

    //printf("mesage %s de type %s\n", stSR, stType);
    }

