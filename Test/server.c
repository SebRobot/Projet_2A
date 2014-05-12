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

typedef struct {
    int date;
    float x;    //position x
    float y;    //position y
    }msgCmd; 

typedef struct {
    int date;
    int bat;  //battery
    float son;  //sonar
    float x;    //position x
    float y;    //position y
    }msgStatus; 

msgStatus msg;

int main(void){
    int listenfd = 0, connfd = 0;
    int n=0, i;
    struct sockaddr_in serv_addr; 

    msg.date = 4;
    msg.bat = 5;

    char sendBuff[1025], bufRec[512];
    time_t ticks; 

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff)); 
    memset(sendBuff, '0', sizeof(bufRec)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000); 

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(listenfd, 10); 

    while(1){
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); //fonction bloquante

        printf("loc 4 bat : %d\n", msg.bat);

        //recv(connfd, &msg, sizeof(msg), 0);
        printf("bat : %d\n", msg.bat);
        send(connfd, &msg, sizeof(msg), 0);

        //ticks = time(NULL);
        //snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
       // snprintf(sendBuff, sizeof(sendBuff), "Bonjour"); //write our message
        //write(connfd, sendBuff, strlen(sendBuff)); 

        sleep(1);
        
        }
}
