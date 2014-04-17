#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "i2c.h"


pthread_mutex_t mutexI2C = PTHREAD_MUTEX_INITIALIZER;

void writeData(int fic, char *buf, int nbytes){
	int bytes;

	if((bytes=write(fic, buf, nbytes)) == -1){
		perror("writeData: Error while trying to write data - ");
	}
	else if(bytes != nbytes){
		printf("Warning : Only %u bytes written out of %u requested\n", bytes, nbytes);
	}
}



void readData(int fd, char *buf, int nbytes){
	int bytes;

	if((bytes = read(fd, buf, nbytes)) == -1){
		perror("readData: Error while trying to read data - ");
    }
	else if(bytes != nbytes){
		printf("Warning : Only %u bytes read out of %u requested\n", bytes, nbytes);
    }
}

void sendCom(char bufW[], char bufR[], int sizeW, int sizeR, int fd){

   if( pthread_mutex_lock(&mutexI2C) != 0){
        printf("Error mutex lock\n");        
        }

    if(sizeW!=0){
        writeData(fd, bufW, sizeW);
        }
    if(sizeR!=0){
        readData(fd, bufR, sizeR);
        }

   if( pthread_mutex_unlock(&mutexI2C) != 0){
        printf("Error mutex lock\n");        
        }
    }



int setI2C(int addr, int fd){


	 if (ioctl(fd, I2C_SLAVE, addr) == -1){
		perror("Failed to acquire bus access and/or talk to slave.");
		return(-1);
	}
	return 0;
}



int openI2C(void){
    int fd;

    if ((fd = open(FILE_I2C, O_RDWR)) == -1){
        perror("openPort: Unable to open - ");
        return(-1);
    }
    else{  
    	if (ioctl(fd, I2C_SLAVE, ADDR) < 0){
        	perror("Failed to acquire bus access and/or talk to slave.");
        	return(-1);
        }
     }
    return (fd);
}



void closeI2C(int fd){
	if(close(fd) == -1){
        perror("closePort: Unable to close - ");
    }
}
