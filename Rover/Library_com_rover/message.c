



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
#include <pthread.h>


#include "message.h"



sInfos sInfoRover;
sArgThrdSttCom argThreadSttRover;
sPosit position;
eTypeCmd typ_Cmd = STATE;
eSta order = STP;


// Lock order, position, typ_Cmd and ArgStt
pthread_mutex_t mtx_order = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mtx_position = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mtx_typ_Cmd = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mtx_ArgStt;



int init_connection(struct sockaddr_in server){
    int sock, yes = 1;

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

	// Avoid "Address already in use" when prgm launched afer abrupt end
	  if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		perror("setsockopt");
		pthread_exit(NULL);
	  }
	  
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

	printf("sock end init = %d\n", sock);
    return sock;
}


void accept_com(){
	int sock, client_sock = 0, c, *new_sock;
	struct sockaddr_in server, client;
	pthread_t processCltThrd, sendInfoThrd;

	sock = init_connection(server);

    // Accept and incomming connection
    printf("Waiting incomming connection...\n");
    c = sizeof(struct sockaddr_in);
    
    // Accept connection from an incomming client
    while((client_sock = accept(sock, (struct sockaddr*)&client, (socklen_t*)&c)) > 0){
	   printf("Connection accepted: client_sock = %d\n", client_sock);
	   
       // Creation of thread to deal request of client
       new_sock = malloc(1);
       *new_sock = client_sock;
       if(pthread_create(&processCltThrd, NULL, connection_handler, (void*) new_sock) < 0){
            perror("could not create thread");
            exit(1);
        }
        
        // Display IP of client and control station
        char*z;
        in_addr_t x;
        x = (in_addr_t)(inet_addr((char*) ADDR_CTRL_PC));
        z = inet_ntoa(*(struct in_addr*)&x);
        printf("inet_addr((char*) ADDR_CTRL_PC = %s\n", z);
        z = inet_ntoa(*(struct in_addr*)&(client.sin_addr.s_addr));
        printf("client.sin_addr.s_addr = %s\n", z);
        
        // Creation of thread to send info Rover to control station
        //if(client.sin_addr.s_addr == (in_addr_t)inet_addr((char*) ADDR_CTRL_PC)){
        	// Set arg for thread
         	argThreadSttRover.clt_sock = client_sock; 
        	if(pthread_create(&sendInfoThrd, NULL, threadSttRover, (void*) &argThreadSttRover) < 0){
		        perror("could not create thread");
		        exit(1);
		    }
        //} 
    }
    if(client_sock < 0){
       perror("accept() failed");
       exit(1);
    }
}


void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    sMsg msgFromClt;
     
	read_size = recv(sock, &msgFromClt, sizeof(sMsg),0);
    //Receive a message from client
	while(read_size > 0){
		#ifdef DBG_CONNECTION_HANDLER
		printf("Message received\n");
		//printf("read_size = %d\n", read_size);
		#endif
		if(read_size > 0){
			// Type of message ?
			printf("msg_in.type = %s\n", dspl_eTypeMsg(msgFromClt.type));
			switch(msgFromClt.type){
				case CMD: 
				          #ifdef DDSPL_RCV_MSG
				          dsplMsg(msgFromClt);
						  #endif
						  // Lock acces to positon, order and typ_Cmd 
					  	  pthread_mutex_lock(&mtx_position);
					  	  pthread_mutex_lock(&mtx_order);
					  	  pthread_mutex_lock(&mtx_typ_Cmd);
					  	  
						  if(msgFromClt.body.cmd.type == TRAJ){
						  	  typ_Cmd = TRAJ;
						      order = MVT;
						      position.pt = msgFromClt.body.cmd.order.traj;
						  }
						  else if(msgFromClt.body.cmd.type == STATE){
						  	  typ_Cmd = STATE;
						      order = msgFromClt.body.cmd.order.state;
						  }
						  else if(msgFromClt.body.cmd.type == POS){
						  	  typ_Cmd = POS;
						      order = msgFromClt.body.cmd.order.state;
						      position.pt = msgFromClt.body.cmd.order.pos.pt;
						      position.ang = msgFromClt.body.cmd.order.pos.ang;
						  }
						  else printf("msgFromClt.body.cmd.type = %s unknown\n", dspl_eTypeCmd(msgFromClt.body.cmd.type));
						  // Unlock
					      pthread_mutex_unlock(&mtx_position);
					      pthread_mutex_unlock(&mtx_order);
					  	  pthread_mutex_unlock(&mtx_typ_Cmd);
					  	  
				          break;
				default: printf("No command message\n"); break;
			}
		}
	read_size = recv(sock, &msgFromClt, sizeof(sMsg),0);
	}
    // Client disconnected
    if(read_size == 0){
        printf("Client disconnected\n");
        
    }
    // Error on connection
    else if(read_size == -1){
        perror("Error in com(): recv() failed");
    }
    //Free the socket pointer
    free(socket_desc);
     
    pthread_exit(NULL);
}


void *threadSttRover(void *sArg){
	int time0 = 0, time1;
	sInfos inf2send;
	sArgThrdSttCom arg = *(sArgThrdSttCom*)sArg;
	
	printf("\nthreadSttRover\n");
	printf("arg.clt_sock = %d\n", arg.clt_sock);
	while(arg.clt_sock > 0){
		time1 = millisec();
		if((time1 - time0) > T_INFOS){
			time0 = time1;
			memcpy(&inf2send, &arg.sinf, sizeof(sInfos));
			send_state(inf2send, arg.clt_sock);
		}
	}
	pthread_exit(NULL);
}

/*
void displayMsgCmd(sMsg msg){
	printf("Message receive\n"); 
}
*/

#ifdef DSPL_MSG
void dsplMsg(sMsg msg){
	if(msg.type == CMD){
		printf("\t msg.type = CMD\n");
		printf("\t msg.body.cmd.num = %d\n", msg.body.cmd.num);
		if(msg.body.cmd.type == TRAJ){
		 printf("\t msg.body.cmd.order.traj = (%.2f; %.2f)\n\n", msg.body.cmd.order.traj.x, msg.body.cmd.order.traj.y);
		}
		else if(msg.body.cmd.type == STATE){
			if(msg.body.cmd.order.state == STP) printf("\t msg.body.cmd.order.state = STOP\n");
			else if(msg.body.cmd.order.state == MVT) printf("\t msg.body.cmd.order.state = MVT\n\n");
			else printf(" type of STATE unknown\n");
		}
		else printf("Type of command unknown\n");
	}
	else if(msg.type == INFO){
		printf("\t msg.type = INFO\n");
		printf("\t msg.body.infos.num = %d\n", msg.body.infos.num);
		printf("\t msg.body.infos.bat = %d\n", msg.body.infos.bat);
		printf("\t msg.body.infos.son = %.2f\n", msg.body.infos.son);
		printf("\t msg.body.infos.pos = (%.2f; %.2f)\n", msg.body.infos.pos.x, msg.body.infos.pos.y);
		printf("\t msg.body.infos.ang = %.2f\n\n", msg.body.infos.ang);
	}
	else printf("Type of message unknown\n");
}
#endif

void send_state(sInfos sinfos, int clt_sock){
	sMsg msg_out;
	#ifdef TELNET
	char text[20];
	#endif
	
	// Send Info
	static uint16_t num = 0;
	msg_out.type = INFO;
	msg_out.body.infos.num = num; num++;
	msg_out.body.infos.bat = (int32_t)sinfos.bat;
	msg_out.body.infos.son = sinfos.son;
	msg_out.body.infos.pos.x = sinfos.pos.x;
	msg_out.body.infos.pos.y = sinfos.pos.y;
	msg_out.body.infos.ang = sinfos.ang;
	#ifdef DSPL_SND_MSG
	dsplMsg(msg_out);
	#endif
	
#ifdef TELNET
	// Send start balise
	sprintf(text,"\nDebut Info\n");
	if(send(clt_sock, text, strlen("\nDebut Info\n"),0) < 0){
		perror("Error in function com() for send() text");
	}
#endif
	// Send message
	if(send(clt_sock, &msg_out, sizeof(eTypeMsg) + sizeof(sInfos),0) < 0){
		perror("Error in function com() for send() msg");
	}
#ifdef TELNET
	// Send end balise 
	sprintf(text,"\nFin Info\n\n");
	if(send(clt_sock, text, strlen("\nFin Info\n\n"),0) < 0){
		perror("Error in function com() for send() text");
	}
#endif
	printf("Message Info sended\n\n"); 
}



void updateInfoRover(sArgThrdSttCom* arg, float x, float y, float theta, float tension, int dist){
	pthread_mutex_lock(&mtx_ArgStt);
	
/*	printf("x = %.2f; y = %.2f; theta = %.2f\n"
		   "bat = %.2f\n"
		   "son = %d\n", x, y, theta, tension, dist);
*/	arg->sinf.bat = tension;
	arg->sinf.son = dist;
	arg->sinf.pos.x = x;
	arg->sinf.pos.y = y;
	arg->sinf.ang = theta;
	
	pthread_mutex_unlock(&mtx_ArgStt);
}


	
