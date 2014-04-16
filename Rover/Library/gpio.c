/*
 * gpio.c
 *
 *  Created on: 15 avr. 2014
 *      Author: Sébastien Malissard
 */


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>




int initGPIO(char *gpio){ //Use the define example : GPIO_50 for "50" ; Warning : example gpio1[28] = 1*32 + 28 = 60
    FILE * pFile;
    if((pFile=fopen("/sys/class/gpio/export", "w"))==NULL){
        perror("Error opening file in initGPIO\n");
        return -1;
        }   

    if(fwrite (gpio , sizeof(char), sizeof(gpio), pFile)!=sizeof(gpio)){
        printf("Warning : miss write char\n");
        }
    if(ferror(pFile)){
        printf("Error writing to file in initGPIO\n");
        return -1;
        }

    fclose(pFile);

    return 0;
    }

int writeGPIO(char *gpio, char *state){ //Only for the moment sizeof(gpio)=2
    FILE * pFile;
    char path[32];

    strcpy (path,"/sys/class/gpio/gpio");
    strcat (path,gpio);
    strcat (path,"/direction");

    if((pFile=fopen(path, "w"))==NULL){
        perror("Error opening file in writeGPIO\n");
        return -1;
        }   

    if(fwrite (state , sizeof(char), sizeof(state), pFile)!=sizeof(state)){
        printf("Warning : miss write char\n");
        }
    if(ferror(pFile)){
        printf("Error writing to file in writeGPIO\n");
        return -1;
        }
 
    fclose(pFile);

    return 0 ;
    }

int readGPIO(char *gpio){ //Only for the moment sizeof(gpio)=2
    FILE * pFile;
    char path[28];
    char ret;

    strcpy (path,"/sys/class/gpio/gpio");
    strcat (path,gpio);
    strcat (path,"/value");

    if((pFile=fopen(path, "r"))==NULL){
        perror("Error opening file in writeGPIO\n");
        return -1;
        }   

    if(fread (&ret , sizeof(char), sizeof(char), pFile)!=sizeof(char)){
        printf("Warning : miss write char\n");
        }
    if(ferror(pFile)){
        printf("Error reading to file in readGPIO\n");
        return -1;
        }
 
    fclose(pFile);

    return atoi(&ret) ;
    }


/*
int main(){


    //Activation du gpio
    initGPIO(GPIO_50);
    initGPIO(GPIO_51);

    writeGPIO(GPIO_50, "high");


    writeGPIO(GPIO_60, "in");
while(1){
if(readGPIO(GPIO_60)==1)    writeGPIO(GPIO_51, "low");
else     writeGPIO(GPIO_51, "high");

}

    return 1;
    }

*/
