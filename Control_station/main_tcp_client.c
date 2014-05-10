//by Sébastien Malissard

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

int main(int argc, char *argv[]){
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr; 


    memset(recvBuff, '0',sizeof(recvBuff)); //init buffer

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){ //creation of the socket
        printf("\n Error : Could not create socket \n");
        return 1;
        } 

    memset(&serv_addr, '0', sizeof(serv_addr)); //init serv_addr

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(631); 

    if(inet_pton(AF_INET, "192.168.0.1", &serv_addr.sin_addr) <= 0){ //convert string character on ip adress
        printf("\n inet_pton error occured\n");
        return 1;
        } 

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
       printf("\n Error : Connect Failed \n");
       return 1;
        } 

    if(write(sockfd, "GET / HTTP/1.1\r\nHost: seurat\r\n\r\n", strlen("GET / HTTP/1.1\r\nHost: seurat\r\n\r\n")) < 0){
        printf("erreur: write()\n");
    }

    while ( (n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0){
        recvBuff[n] = 0;
        if(fputs(recvBuff, stdout) == EOF){
            printf("\n Error : Fputs error\n");
            }
        } 

    if(n < 0){
        printf("\n Read error \n");
        } 

    return 0;
}
