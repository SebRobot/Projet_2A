


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
#include "../../typeMessage.h"
#include "../Library/param.h"


#define PORT           5000
#define MAX_CONNECTION 2
#define MAX_CLIENTS    1
#define ADDR_CTRL_PC "192.168.1.174" // @IP of control station à corriger
/*
typedef struct{
     float x;
     float y;
} sPt;

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
*/

typedef struct sArgThrdSttCom sArgThrdSttCom;
struct sArgThrdSttCom{
	int clt_sock;
	sInfos sinf;
};


extern sArgThrdSttCom  argThreadSttRover; 
extern sPosit position;
extern eTypeCmd typ_Cmd;
extern eSta order;

// Lock order and ptTraj
extern pthread_mutex_t mtx_order;
extern pthread_mutex_t mtx_ptTraj;
extern pthread_mutex_t mtx_position;
extern pthread_mutex_t mtx_typ_Cmd;

int init_connection(struct sockaddr_in server);
void accept_com();
void *connection_handler(void *socket_desc);
void *threadSttRover(void *sArg);
void dsplMsg(sMsg msg);
void send_state(sInfos sinfos, int clt_sock);
void updateInfoRover(sArgThrdSttCom* arg, float x, float y, float theta, float tension, int dist);



#endif //MESSAGE_H
