//by Sébastien Malissard

#ifndef PARAM_H
#define PARAM_H

#include "type.h"

#define NB_ROBOT 10
#define PORT     5000
#define IP       "127.0.0.1"


//TODO sous forme d'un fichier de routage
#define IP_ROBOT_1   "127.0.0.1"
#define IP_ROBOT_2   "127.0.0.1"
#define IP_ROBOT_3   "127.0.0.1"
#define IP_ROBOT_3   "127.0.0.1"
#define IP_ROBOT_4   "127.0.0.1"



extern infoRobot robot[];

int rwRobot(eRW rw, int num, infoRobot *p);

#endif
