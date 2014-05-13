//by Sébastien Malissard

#ifndef MENU_H
#define MENU_H

void init(void);
void wait(void);
int enterChar(int nb, char *ret);
int addRobot(void);
int selectRobot(void);
void menu(int num);
void displayRobot(void);
void sendPoint(int num);
void rmRobot(int id);

#endif
