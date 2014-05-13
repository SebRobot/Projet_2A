//by Sébastien Malissard

#ifndef COM_H
#define COM_H

#include "type.h"

#include "../typeMessage.h"

int initCom(const char *ip);
int senMsg(const int num, const sMsg *msg);
void *moniRecv(void *socket);
void saveHistMsg(eTypeMsg type, eSendRecv sr);


#endif
