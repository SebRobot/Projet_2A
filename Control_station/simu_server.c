//by Sébastien Malissard

#include "../typeMessage.h"
#include "param.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 

    sMsg msgOut, msgIn;

int main(void){
    int sock = 0, conn = 0;
    int n=0, i, ret;
    struct sockaddr_in serv_addr; 

    sock = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(PORT); 
    if(sock < 0){
        perror("socket()");
        exit(1);
        }

    ret = bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if(ret < 0){
        perror("bind()");
        exit(1);
        }

    ret = listen(sock, 10); 
    if(ret < 0){
        perror("listen()");
        exit(1);
        }

    msgOut.type = CMD;
    msgOut.body.cmd.date = 0;

    while(1){
        conn = accept(sock, (struct sockaddr*)NULL, NULL); //fonction bloquante
        if(conn < 0){
            perror("accept()");
            exit(1);
            }
        printf("Connection établie\n");
        while(1){
                ret = recv(conn, &msgIn, sizeof(msgIn), 0);
                if(ret < 0){
                    perror("recv()");
                    exit(1);
                    }
                else if(ret == 0){
                    conn = -1;
                    break;
                    }

                printf("message reçu : x =%f\n", msgIn.body.cmd.pt.x);
                ret = send(conn, &msgOut, sizeof(msgOut), 0);
                if(ret < 0){
                    perror("send()");
                    exit(1);
                    }

                printf("message envoyé\n");   
                msgOut.body.cmd.date++;                 
                }
        sleep(1);
        }
    }
