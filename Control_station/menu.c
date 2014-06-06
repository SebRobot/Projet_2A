//by Sébastien Malissard

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "menu.h"

#include "type.h"
#include "param.h"
#include "tools.h"
#include "com.h"
#include "../typeMessage.h"
#include "Library/formatting_text.h"


void init(void){
    int i;
    infoRobot rob;

    for(i = 0 ; i < NB_ROBOT ; i++){
        rob.loc = FREE;
        rob.num = 0;
        memset(&rob.ip[0], '\0', sizeof(rob.ip));
        rob.sock = 0;
        rob.date = 0;
        rob.bat = 0.;
        rob.pos.x = 0.;
        rob.pos.y = 0.;
        rob.son = 0.;
        rob.state = STP;
        rwRobot(WRITE, i, &rob);
        }

    #ifdef DEBUG
    displayRobot();
    #endif

    printf(EF_BOLT BG_RED"\n---------------- Bienvenue ! -----------------" DEFAULT "\n");
    printf(EF_BOLT "par Sébastien MALISSARD & Yoann SOLANA\n" DEFAULT);
    printf("version 1.0 : 05/2014\n");
    printf("\nInformation : Le nombre de robots maximal est de : %d\n\n", NB_ROBOT);
    }


int addRobot(void){
    int ret = 0;
    infoRobot rob = {0};
    char buf[36];

    printf("Saisir le numéro : ");
    if( (ret = enterNum()) == -1){
        return -1;
        }

    if(ret > 9 || ret < 1){
        printf("Le nombre doit etre compris entre 1 et 9\n");
        return -1;
        }

    printf("Saisir l'adresse ip : ");
    if(enterIP(&rob.ip[0]) == -1){
        return -1;
        }

    rob.loc = ACTIVE;
    rob.num = ret;

    rwRobot(WRITE, ret, &rob);

    printf("Le robot n°%d est activé\n", ret);

    if( (rob.sock = initCom(&rob.ip[0])) > 0){
        sprintf(buf, "La connexion avec le robot n°%d est établie\n", ret);
        printf("%s\n", buf);
        printConsole(buf);
        }
    else{
        rob.sock = -1;
        rwRobot(WRITE, ret, &rob);
        printf("La connexion avec le robot n°%d n'a pas pu être établie\n", ret);
        return -1;
        }

    #ifdef DEBUG
    printf("socket = %d\n", rob.sock);
    #endif

    if(rwRobot(WRITE, ret, &rob) == -1){
        printf("Error to write data\n");
        return -1;
        }
    return rob.num;
    }

void rmRobot(int id){

    };

int selectRobot(void){
    int ret = 0;
    
    printf("Saisir le numéro : "); //TODO more 10 robots
    ret = enterNum();
    if(ret > 9 || ret < 1){
        printf("Le nombre doit etre compris entre 1 et 9\n");
        return -1;
        }

    if(robot[ret].loc == FREE){
        printf("Le robot n'est pas définie (- a Ajouter un robot)\n");
        return -1;
        }

    printf("Le robot n°%d est sélectionné\n", ret);

    return ret;
    }

void displayRobot(void){
    int i, ret;
    char str[32] = "";
    infoRobot rob = {0};

    printf("Numéros |       ip       | baterie  |  position (x, y)  |  sonar\n");
    printf("------------------------------------------------------------------\n");
    for(i = 1 ; i < NB_ROBOT ; i++){
        ret = rwRobot(READ, i, &rob);
        if( ret>0 && rob.loc == ACTIVE){
            sprintf(str, "%d", rob.num);
            printLeft(str, 8);
            printf("|");
            printf(" %-15s", rob.ip);
            printf("|");
            sprintf(str, "%.1f", rob.bat);
            printLeft(str, 10);
            printf("|");
            sprintf(str, "%.1f", rob.pos.x);
            printLeft(str, 9);
            sprintf(str, "%.1f", rob.pos.y);
            printLeft(str, 10);
            printf("|");
            sprintf(str, "%.1f", rob.son);
            printLeft(str, 10);
            printf("\n");   
            }
        }
    }

void msgConsol(void){
	char namePipe[] = "essai.fifo";
    int id;

    remove(namePipe);

    if(mkfifo(namePipe, 0644) != 0) {
        perror("Error to create the named pipe ");
        exit(EXIT_FAILURE);
        }

    id = fork();

    if(id == 0){ //child processus
        execlp("gnome-terminal", "./gnome-terminal", "-e", "/home/seb/RobotOfficiel/Projet_2A/Control_station/console essai.fifo", NULL); //FIXME
        }

    else{ //father processus
        if((pipeConsole = open(namePipe, O_WRONLY)) == -1){
	        perror("Error to open the named pipe\n");
	        exit(EXIT_FAILURE);
            }
        printConsole("Bienvenue sur la console de supervision des messages\n\n");
        
        printf("Lancement de la console\n");
        }
    }

int sendPoint(int num){
    int x, y;
    sMsg msg = {'\0'};

    printf("x = ");
    if( (x = enterNum()) == -1){
        return -1;
        }
    printf("y = ");
    if( (y = enterNum()) == -1){
        return -1;
        }

    msg.type = CMD;
    msg.body.cmd.type = TRAJ;
    msg.body.cmd.order.traj.x = x;
    msg.body.cmd.order.traj.y = y;

    if(senMsg(num, &msg) == -1){
        return -1;
        }

    return 0;
    }

void stopRover(int num){
    sMsg msg = {'\0'};

    msg.type = CMD;
    msg.body.cmd.type = STATE;
    msg.body.cmd.order.state = STP;

    senMsg(num, &msg);
    }

void menu(int num){
    printf(EF_BOLT EF_UNLINE TXT_RED"\nMENU\n" DEFAULT);


    printf("|"EF_BOLT" s "DEFAULT": Sélectionner un robot\n");
    printf("|"EF_BOLT" a "DEFAULT": Ajouter un robot\n");
    printf("|"EF_BOLT" l "DEFAULT": Afficher la liste des robots\n"); //TODO
    printf("|"EF_BOLT" c "DEFAULT": Afficher les messages de communication\n"); //TODO
    printf("|"EF_BOLT" q "DEFAULT": Quitter\n");

    if(num != 0){
        printf("|\n");
        printf("| Robot n°%d sélectionné :\n", num);
        printf("|"EF_BOLT"     i "DEFAULT": Informations\n");
        printf("|"EF_BOLT"     e "DEFAULT": Envoyer un point de destination\n");
        printf("|"EF_BOLT"     h "DEFAULT": Arrêter le robot\n");
        printf("|"EF_BOLT"     n "DEFAULT": Associer un nom\n");
        printf("|"EF_BOLT"     r "DEFAULT": Supprimer\n");
        printf("|"EF_BOLT"     d "DEFAULT": Déselectionner\n");
        }

    printf(TXT_GREEN "Choix : " DEFAULT);    
    }
