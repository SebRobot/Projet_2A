/************************************************
*I2C example code for I2C-1 on BBB and MD25 	*
*                                               *
*Compiled using gcc				                *
*                                               *
*By Sébastien Malissard and Yoann Solana        *
*On 23 March 2014       			            *
************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <linux/i2c-dev.h>

#define P_ERROR	-1						// to check for errors when dealing with the port
#define ADDR 0x58                       //Address of SFR10 (default : 0x70)
#define SPEED1  0x00
#define SPEED2  0x01
#define TURN    0x01
#define VOLT    0x0A
#define ENC1    0x02
#define ENC2    0x06
#define CMD     0x10
#define MODE    0x0f
#define COM     0x10

int openPort(void);
void closePort(int fd);
void writeData(int fd, int nbytes);
void readData(int fd, int nbytes);


unsigned char sbuf[10];						// Stores data to be read and written

int main(int argc, char *argv[])
    {
    int fd;								    // file descriptor of open port
    int range;
    int light;
    int ver;
    char rbuf[10];
    unsigned int d;

    fd = openPort();						// Open i2c	

    if(fd > P_ERROR)						// Only do the folloing if there has not been an error opening the port
        {
        //Initialize encoder
        sbuf[0] = COM;
	    sbuf[1] = 0x20;												
	    writeData(fd, 2);

        sbuf[0] = MODE;
 	    sbuf[1] = 0x03;						
	    writeData(fd, 2);       

	    sbuf[0] = SPEED1;						
	    sbuf[1] = -100;						
	    writeData(fd, 2);	

        sbuf[0] = TURN;						
	    sbuf[1] = 0x00;						
	    writeData(fd, 2);	

        sbuf[0] = VOLT;											
	    writeData(fd, 1);	
        readData(fd, 1);
        printf("Voltage : %.1f V\n",(float)sbuf[0]/10);		
   
        sleep(1);

        //Encoder 1
        sbuf[0] = ENC1;											
	    writeData(fd, 1);
        readData(fd, 1); 
        d = sbuf[0]<<24;
        readData(fd, 1); 
        d |= sbuf[0]<<16;
        readData(fd, 1); 
        d |= sbuf[0]<<8;
        readData(fd, 1); 
        d |= sbuf[0];
        printf("nb_tour_enc1=%i\n", (int)d/10800);

        //Encoder 2
        sbuf[0] = ENC2;											
	    writeData(fd, 1);
        readData(fd, 1); 
        d = sbuf[0]<<24;
        readData(fd, 1); 
        d |= sbuf[0]<<16;
        readData(fd, 1); 
        d |= sbuf[0]<<8;
        readData(fd, 1); 
        d |= sbuf[0];
        printf("nb_tour_enc2=%i\n", (int)d/10800);
        }

    closePort(fd);	
    return 0;
    }


int openPort(void)
    {
    int fd;										                // File descriptor for the port

    fd = open("/dev/i2c-1", O_RDWR);				            // default : /dev/i2c-1 on P9 with 19 : SCL and 20 : SDA
    if (fd == P_ERROR)
        {
	    perror("openPort: Unable to open /dev/ttyUSB0 - ");		// If open() returns an error
        } 
    else
        {
        if (ioctl(fd, I2C_SLAVE, ADDR) < 0)
            {
        perror("Failed to acquire bus access and/or talk to slave.");
        return(-1);
            }
        }
    return (fd);
    }


void closePort(int fd)
    {	
	if(close(fd) == P_ERROR)						// Close the port if it returns an error then display an error report
	    {	
		perror("closePort: Unable to close /dev/ttyUSB0 - ");
	    }
    }


void writeData(int fd, int nbytes)
    {
    int bytes;
	
	bytes = write(fd, sbuf, nbytes);				// Write nbytes of data from wbuf
	if(bytes == P_ERROR)							// If write returns an error (-1)
	    {
		perror("writeData: Error while trying to write data - ");	
	    }
	else if(bytes != nbytes)
	    {
		printf("only %u bytes written out of %u requested\n", bytes, nbytes);	
	    }
    }


void readData(int fd, int nbytes)
    {
    int bytes;
	bytes = read(fd, sbuf, nbytes);				    // Read nbytes of data into rbuf
	if(bytes == P_ERROR)							// If read returns and error (-1)
	    {
		perror("readData: Error while trying to read data - ");
	    }
	else if(bytes != nbytes)
	    {
		printf("Only %u bytes read out of %u requested\n", bytes, nbytes);
	    }
    }


