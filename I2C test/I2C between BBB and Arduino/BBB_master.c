//By Sébastien Malissard
//22 March 2014
// e.linux.org/Interfacing_with_I2C_Devices

//put pull-uo resitor for example 1.8k

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define ADDR        0x0a            //between 0x03 and 0x77
#define I2C_BUS     "/dev/i2c-1"    //i2c bus used
#define MODE        0               //0 : send a charactere, 1 : read a charactere

int main()
    {
    int fileId;
    char *fileName = I2C_BUS; 
    char data='F';

    //Opening the Bus
    if ((fileId = open(fileName, O_RDWR)) < 0) 
        {
        perror("Failed to open the i2c bus");
        exit(-1);
        }

    //Initiating communication with Arduino
    if (ioctl(fileId, I2C_SLAVE, ADDR) < 0)
        {
        perror("Failed to acquire bus access and/or talk to slave.");
        exit(-1);
        }

    while(1)
        {
        switch(MODE)
            {
            case(0) : 
                if(write(fileId,&data,1) < 0) perror("Failed to write from the i2c bus.");
                else printf("Succes to write : %c\n", data);
                break;
            default :
                break;
            }
        sleep(1);
        }
    }
