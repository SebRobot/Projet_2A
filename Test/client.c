//http://www.thegeekstuff.com/2011/12/c-socket-programming/

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 

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

int main(){
    int sockfd = 0, n = 0;
    char buf[1024];
    struct sockaddr_in serv_addr; 

    msg.date=999;
    msg.bat=999;
    memset(buf, ' ', sizeof(buf));

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("\n Error : Could not create socket \n");
        return -1;
        } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000); 

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0){
        printf("\n inet_pton error occured\n");
        return 1;
        } 



    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
       printf("\n Error : Connect Failed \n");
       return 1;
       }

    printf("date loc 999 : %d\n", msg.date);
    send(sockfd, &msg, sizeof(msg), 0);
//while(1){
    recv(sockfd, &msg, sizeof(msg), 0);
    printf("date : %d\n", msg.date);

        sleep(1);
//}

    return 0;
    }
