//by Sébastien Malissard

#ifndef TYPEMESSAGE_H
#define TYPEMESSAGE_H

#include <stdint.h>

typedef enum {
    SEND,
    RECV   
    } eSendRecv;

typedef enum {
    CMD,
    INFO
    } eTypeMsg;

typedef struct {
    int date;
    sPt pt;     //position
    } sCmd; 

typedef struct {
    uint16_t num;
    int32_t bat;    //10* voltage battery
    float son;      //sonar
    sPt pos;        //position
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
