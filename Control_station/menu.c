//by Sébastien Malissard

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "menu.h"

#include "type.h"
#include "param.h"
#include "tools.h"
#include "com.h"
#include "../typeMessage.h"


void init(void){
    int i, j;

    for(i = 0 ; i < NB_ROBOT ; i++){
        robot[i].loc = FREE;
        for(j = 1 ; j < 20 ; j++){
            robot[i].ip[j] = '\0';
            }
        }

    printf("\n------------ Bienvenue ! -------------\n");
    printf("par Sébastien MALISSARD & Yoann SOLANA\n");
    printf("version 1.0 : 05/2014\n");
    printf("\nInformation : Le nombre de robots maximal est de : %d\n\n", NB_ROBOT);
    }


int addRobot(void){
    int ret = 0;
    char *ip;
    infoRobot rob;

    printf("Saisir le numéro : ");
    ret = enterNum();
    if(ret > 9){
        printf("Le nombre doit etre compris entre 0 et 9\n");
        return -1;
        }

    rwRobot(READ, ret, &rob);

    printf("Saisir l'adresse ip : ");
    ip = &rob.ip[0];
    if(enterIP(ip) == -1){
        return -1;
        }
    rob.loc = ACTIVE;
    rob.num = ret;

    printf("Le robot n°%d est activé\n", ret);

    if( (rob.sock = initCom(&rob.ip[0])) > 0){
        printf("La connexion avec le robot n°%d est éablie\n", ret);
        }

    rwRobot(WRITE, ret, &rob);

    return 1;
    }

void rmRobot(int id){


    };

int selectRobot(void){
    int ret = 0;
    char c;
    
    printf("Saisir le numéro : "); //TODO more 10 robots
    enterString(1, &c);
    if( c >= '0' && c <= '9'){
        ret = c - '0';
        }
    else{
        printf("Erreur de syntax\n");
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
    int i;

    printf("Numéros |    ip    | baterie | position (x, y) | sonar\n");
    for(i = 0 ; i < NB_ROBOT ; i++){
        if( robot[i].loc == ACTIVE){
            printf("   %d   |", robot[i].num);            
            printf("%s|", robot[i].ip);       
            printf("  %f  |", robot[i].bat);       
            printf("%f, ", robot[i].pos.x);
            printf("%f|", robot[i].pos.y);       
            printf("%f  ", robot[i].son);
            printf("\n");       
            }
        }
    }

void sendPoint(int num){
    int x, y;
    sMsg msg;

    x = enterNum();
    y = enterNum();

    msg.type = CMD;
    msg.body.cmd.pt.x = x;
    msg.body.cmd.pt.y = y;

    senMsg(num, &msg);
    }

void menu(int num){
    printf("\nMenu\n");
    printf("| -s Sélectionner un robot\n");
    printf("| -a Ajouter un robot\n");
    printf("| -l Afficher la liste des robots\n"); //TODO
    printf("| -c Afficher les messages de communication\n"); //TODO
    printf("| -q Quitter\n");

    if(num != 0){
        printf("|\n");
        printf("| Robot n°%d sélectionné :\n", num);
        printf("|     -i Informations\n");
        printf("|     -e Envoyer un point de destination\n");
        printf("|     -n Associer un nom\n");
        printf("|     -r Supprimer\n");
        printf("|     -d Déselectionner\n");
        }

    printf("Choix : ");    
    }
