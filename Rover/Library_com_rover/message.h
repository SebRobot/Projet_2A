


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

#include "../Library/tools.h" 


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



extern int init_connection(struct sockaddr_in server);
extern void com(sInfos sinfos);
extern void displayMsgCmd(sMsg msg);

#endif //MESSAGE_H
