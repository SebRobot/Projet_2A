//by Sébastien Malissard

#ifndef TYPEMESSAGE_H
#define TYPEMESSAGE_H

#include <stdint.h>

typedef struct{
    float x;
    float y;
    } sPt;

typedef struct{
    sPt pt;
    float ang;
    }sPosit;

typedef enum {SEND,RECV} eSendRecv;
typedef enum {CMD,INFO} eTypeMsg;
typedef enum {TRAJ, STATE, POS} eTypeCmd;
typedef enum {STP, MVT} eSta;


typedef union{
	sPt traj;
	eSta state;
    sPosit pos;
    }uOrder;

typedef struct {
    uint16_t num;
    eTypeCmd type;
    uOrder order;
    } sCmd; 

typedef struct {
    uint16_t num;
    int32_t bat;    //10* voltage battery
    float son;      //sonar
    sPt pos;        //position
    float ang;
    } sInfos; 

typedef union {
    sCmd cmd;
    sInfos infos;
    } uBodyMsg;

typedef struct {
    eTypeMsg type;
    uBodyMsg body;
    } sMsg;

#endif
