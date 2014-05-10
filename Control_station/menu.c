//by Sébastien Malissard

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "menu.h"

#include "type.h"
#include "param.h"


void init(void){
    int i;

    for(i = 0 ; i < NB_ROBOT ; i++){
        robot[i].loc = FREE;
        }

    printf("\n------------ Bienvenue ! -------------\n");
    printf("par Sébastien MALISSARD & Yoann SOLANA\n");
    printf("version 1.0 : 05/2014\n");
    printf("\nInformation : Le nombre de robots maximal est de : %d\n\n", NB_ROBOT);
    }

void wait(void){
    printf("Appuyer sur une touche pour continuer\n");
    getchar();
    }

int enterChar(int nb, char *ret){
    char s[64] = {0};
    int i;

    if(nb > 63){
        return -1;
        }

    fgets(s, 64, stdin);

    if(strlen(s) > nb + 1){
        printf("Erreur de syntaxe\n\n");
        printf("Appuyer sur une touche pour continuer\n");
        while(getchar()!='\n');
        return -1;
        }

    for(i = 0 ; i < nb ; i++){
        ret[i] = s[i]; 
        }

    return 1;
    }

int addRobot(void){
    int ret = 0;
    char c;
    
    printf("Saisir le numéro : ");
    enterChar(1, &c);
    if( c >= '0' && c <= '9'){
        ret = c - '0';
        }
    else{
        printf("Erreur de syntax\n");
        return -1;
        }

    robot[ret].loc = ACTIVE;
    printf("Le robot n°%d est activé\n", ret);

    return 1;
    }

int selectRobot(void){
    int ret = 0;
    char c;
    
    printf("Saisir le numéro : "); //TODO more 10 robots
    enterChar(1, &c);
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

void menu(int num){
    printf("\nMenu\n");
    printf("| -s Sélectionner un robot (0 désélectionner)\n");
    printf("| -a Ajouter un robot\n");
    printf("| -q Quitter\n");

    if(num != 0){
        printf("|\n");
        printf("| Robot n°%d sélectionné :\n", num);
        printf("|     -i Informations\n");
        printf("|     -n Associer un nom (en cour de dévelopement)\n");
        printf("|     -r Supprimer le robot\n");
        }

    printf("Choix : ");    
    }
