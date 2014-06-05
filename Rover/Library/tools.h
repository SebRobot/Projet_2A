#ifndef TOOLS_H
#define TOOLS_H


#include "../../typeMessage.h"



extern unsigned int microsec();
extern unsigned int millisec();
char* dspl_eTypeMsg(eTypeMsg eType);
char* dspl_eTypeCMd(eTypeCmd eType);
char* dspl_eSta(eSta eType);



#endif // TOOLS_H
