//by Sébastien Malissard

#include "formatting_text.h"

#include <stdio.h>
#include <string.h>

void printCenter(char *txt, int nb){ 
    int l, i;
    char str2[32] = "";
    l = strlen(txt);

    for(i = 0 ; i < (nb - l)/2 ; i++){
        strcat(str2, " ");
        }
    strcat(str2, txt);
    for(i = 0 ; i < (nb - l)/2 ; i++){
        strcat(str2, " ");
        }

    if( l/2 ) strcat(str2, " ");

    printf(".%s.",str2);
    }

void printLeft(char *txt, int nb){ 
    int l, i;
    char str[32] = "";
    l = strlen(txt);
        
    strcat(str, " ");
    strcat(str, txt);

    for(i = 0 ; i < (nb - l - 1); i++){
        strcat(str, " ");
        }

    printf("%s",str);
    }
