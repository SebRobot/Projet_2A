//by Sébastien Malissard

#ifndef _TYPE_H
#define _TYPE_H

#include "../typeMessage.h"

typedef enum {ACTIVE, FREE} eLoc;
typedef enum {READ, WRITE} eRW;

typedef struct {
    eLoc loc;
    int num;
    char ip[20];
    int sock;
    int date;
    float bat;
    sPt pos;
    float ang;
    float son;
    eSta state;
    } infoRobot;

#endif
