



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h> 


#include "message.h"

#define PORT           5000
#define MAX_CONNECTION 2
#define MAX_CLIENTS    1
#define T_INFOS        100 // Périod 100ms for send infos




int init_connection(struct sockaddr_in server){
   int sock;
//   server = {0};

   // Create socket
   if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
      perror("socket()");
      return -1;
   }
   printf("socket created\n");

   // Set sockaddr_in srtuture
   server.sin_addr.s_addr = htonl(INADDR_ANY); // to verify
   server.sin_port = htons(PORT);
   server.sin_family = AF_INET;

   // Bind
   if(bind(sock,(struct sockaddr *) &server, sizeof(server)) < 0 ){
      perror("bind()");
      return -1;
   }
   printf("bind done\n");

   // Listen
   if(listen(sock, MAX_CLIENTS) < 0){
      perror("listen()");
      return -1;
   }
   printf("Listen done\n");

   return sock;
}


void com(sInfos sinfos){
   int sock,  client_sock, read_size;
   struct sockaddr_in server, client;
   static int time1 = 0, time0 = -1;
   sMsg msg_in, msg_out;

   sock = init_connection(server);
   
   if(time0 < 0) time0 = millisec();
		int c;
		
        // Accept and incomming connection
        printf("Waiting for incomming connection...");
        c = sizeof(struct sockaddr_in);
        
        // Accept connection from an incomming client
        if((client_sock = accept(sock, (struct sockaddr*)&client, (socklen_t*)&c)) < 0){
           perror("accept() failed");
           exit(1);
        }
        printf("Connection acceted\n");
        
        // Receive message from client
        while((read_size = recv(client_sock, &msg_in, sizeof(sMsg),0)) > 0){
            // Type of message ?
            switch(msg_in.type){
            	case CMD: displayMsgCmd(msg_in); break;
            	default: printf("no command message\n"); break;
            }
            time1 = millisec();
            if((time1 - time0) > T_INFOS){
				static uint16_t num = 0;
				time0 = time1;
				msg_out.type = INFO;
				msg_out.bodyMsg.infos.num = num; num++;
				msg_out.bodyMsg.infos.bat = (int32_t)sinfos.bat;
				msg_out.bodyMsg.infos.son = sinfos.son;
				msg_out.bodyMsg.infos.pos.x = sinfos.pos.x;
				msg_out.bodyMsg.infos.pos.y = sinfos.pos.y;
				msg_out.bodyMsg.infos.dir = sinfos.dir;
				send(client_sock, &msg_out, sizeof(eTypeMsg) + sizeof(sInfos),0); 
				char text[10];
				sprintf(text,"Info");
				send(client_sock, text, strlen("Info"),0); 
			}
			time1 = millisec();
        }
        
        // Client deconnected
        if(read_size == 0){
            printf("Client disconnected\n");
        }
        else if(read_size < 0){
            perror("recv() failed");
        }
}


void displayMsgCmd(sMsg msg){
	printf("Message receive\n"); 
}



