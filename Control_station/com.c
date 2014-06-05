//by Sébastien Malissard

#include "com.h"

#include "param.h"
#include "tools.h"
#include "menu.h"

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

int sockMax = 0;


int initCom(const char *ip){
    struct sockaddr_in serv_addr;  
    int sock, i;   

    memset(&serv_addr, '\0', sizeof(serv_addr)); 

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

    if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){ //echec sock = -1
       perror("Error : Connect Failed \n");
       return -1;
       }

    if(sock > sockMax) sockMax = sock; 

    return sock;
    }

int senMsg(const int num, const sMsg *msg){ //num -> listRobot
    infoRobot rob = {0};

    rwRobot(READ, num, &rob);
    printf("sok = %d\n", robot[num].sock);

    if(rob.sock == 0){
        printf("Socket invalide\n");
        }

    if(send(rob.sock, msg, sizeof(*msg), 0) < -1){
        printConsole("Error message not send\n");
        return -1;   
        }

    saveHistMsg(msg->type, SEND);

    return 0;
    }


void *moniRecv(void *ard){
    int ret, i;
    fd_set fd;
    sMsg msg = {'\0'};
    infoRobot rob;
    struct timeval tv;

    #ifdef DEBUG
    printf("Start moniRecv\n");
    #endif


    while(1){
    
        tv.tv_sec = 0;
        tv.tv_usec = 500;

        FD_ZERO(&fd);

        for(i = 0 ; i < NB_ROBOT ; i++){
            rwRobot(READ, i, &rob);
            if( (rob.loc == ACTIVE) && (rob.sock != -1)){
                FD_SET(rob.sock, &fd);
                }
            }

        if( (ret = select(sockMax + 1, &fd, NULL, NULL, &tv)) == -1){
            perror("Error select");
            wait();
            continue;
            }
            
        if( ret == 0) continue; //timer out for update select if new socket
                
        for(i = 0 ; i < NB_ROBOT ; i++){
            rwRobot(READ, i, &rob);
            if( (rob.loc == ACTIVE) && (FD_ISSET(rob.sock, &fd)) ){
                if( (ret = recv(rob.sock, &msg, sizeof(msg), 0 )) == -1){
                    perror("Error recv");
                    wait();
                    continue;
                    }

                saveHistMsg(msg.type, RECV);

                switch(msg.type){  //TODO prendre en compte le message : mise à jour du tableau
                    case CMD :

                        break;
                    case INFO :
                        rob.bat = msg.body.infos.bat;
                        rob.son = msg.body.infos.son;
                        rwRobot(WRITE, i, &rob);
                        break;
                    default :
                        printConsole("Error : type of message unknown\n");
                        break;
                    }  
                }  
            }
        }

    pthread_exit(NULL);
    }

void saveHistMsg(eTypeMsg type, eSendRecv sr){
    switch(sr){
        case SEND :
            if(type == CMD) printConsole("Un message de type CMD a été envoyer\n");
            if(type == INFO) printConsole("Un message de type INFO a été envoyer\n");
            break;
        case RECV :
            if(type == CMD) printConsole("Un message de type CMD reçue\n");
            if(type == INFO) printConsole("Un message de type INFO reçue\n");        
            break;
        default:
            printf("Error in switch sr\n");
        }

    //printf("mesage %s de type %s\n", stSR, stType);
    }

