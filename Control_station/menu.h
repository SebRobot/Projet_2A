//by Sébastien Malissard

#ifndef MENU_H
#define MENU_H

void init(void);
int addRobot(void);
int selectRobot(void);
void menu(int num);
void displayRobot(void);
int sendPoint(int num);
void rmRobot(int id);
void msgConsol(void);
void stopRover(int num);

#endif
