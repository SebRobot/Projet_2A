//by Sébastien Malissard

#ifndef _TYPE_H
#define _TYPE_H


typedef enum {ACTIVE, FREE} eLoc;
typedef enum {STOP, MOVE} eState;
typedef enum {READ, WRITE} eRW;

typedef struct{
    float x;
    float y;
    } sPt;

typedef struct {
    eLoc loc;
    int num;
    char ip[20];
    int sock;
    int date;
    float bat;
    sPt pos;
    float son;
    eState state;
    } infoRobot;

#endif
