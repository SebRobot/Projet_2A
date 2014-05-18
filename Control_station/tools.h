//by Sébastien Malissard

#ifndef TOOLS_H
#define TOOLS_H

#include "type.h"

void printConsole(char *msg);
int rwRobot(eRW rw, int num, infoRobot *p);
void wait(void);
int enterString(int nb, char *ret);
int enterNum(void);
int enterIP(char *ip);

#endif
