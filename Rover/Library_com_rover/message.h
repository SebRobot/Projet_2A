


#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>  // inet_addr
#include <unistd.h>     // write()
#include <pthread.h>

#include "../Library/tools.h" 


#define PORT           5000
#define MAX_CONNECTION 2
#define MAX_CLIENTS    1
#define T_INFOS        5000 // Périod 500ms for send infos
#define ADDR_CTRL_PC "127.0.0.1" // @IP of control station

typedef struct{
     float x;
     float y;
}sPt;
typedef enum{CMD, INFO} eTypeMsg;
typedef enum{TRAJ, ORDER}eTypeCmd;
typedef enum{STOP, MVT} eCmd;
typedef struct{
     uint16_t num;
     int32_t bat;  // 10*tension battery
     float son;    // sonar
     sPt pos;
     float dir;
}sInfos;
typedef union{
	sPt pt;
	eCmd cmd;
}uCmd;
typedef struct{
    uint16_t num;
    eTypeCmd type_cmd;
    uCmd ucmd;
}sCmd;
typedef union{
     sCmd cmd;
     sInfos infos;
}uBodyMsg;
typedef struct {
    eTypeMsg type;
    uBodyMsg bodyMsg;
}sMsg;


typedef struct sArgThrdSttCom sArgThrdSttCom;
struct sArgThrdSttCom{
	int clt_sock;
	sInfos sinf;
};


extern sArgThrdSttCom  argThreadSttRover; 
extern sPt ptTraj;
extern eCmd order;

// Lock order and ptTraj
extern pthread_mutex_t mtx_order;
extern pthread_mutex_t mtx_ptTraj;

int init_connection(struct sockaddr_in server);
void accept_com();
void *connection_handler(void *socket_desc);
void *threadSttRover(void *sArg);
//void *threadSttRover(sArgThrdSttCom *sArg);
void dsplMsg(sMsg msg);
void send_state(sInfos sinfos, int clt_sock);
void updateInfoRover(sArgThrdSttCom* arg, float x, float y, float theta, float tension, int dist);


#endif //MESSAGE_H
