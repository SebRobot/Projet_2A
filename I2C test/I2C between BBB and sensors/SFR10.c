/************************************************
*I2C example code for I2C-1 on BBB and SRF10	*
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
#define ADDR 0x70                       //Address of SFR10 (default : 0x70)

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

    fd = openPort();						// Open i2c	

    if(fd > P_ERROR)						// Only do the folloing if there has not been an error opening the port
        {
	    sbuf[0] = 0x00;						
	    sbuf[1] = 0x51;						
	    writeData(fd, 2);					
        usleep(80000);                      //happy SFR10
        readData(fd, 1);					// Read back the returned byte into sbuf[0] this should be non 0 for success
        rbuf[0]=sbuf[0];                    //Verion of soft
	    if(!sbuf[0])						// If it is 0 report an error
	        {
		    printf("USBI2C error writing to SRF08");
	        }	
        printf("Reg1=%u\n",sbuf[0]);
        

	    sbuf[0] = 0x01;						// USBI2C command for R/W 1 byte address devices
	    writeData(fd, 1);					// Send these 4 bytes to USBI2C
        readData(fd,1);						// Read back 4 bytes, software version, light and high low bytes of range	
        rbuf[1]=sbuf[0];
        printf("Reg2 : %u\n",rbuf[1]);


        sbuf[0] =0x02;
	    writeData(fd, 1);					// Send these 4 bytes to USBI2C
	    readData(fd,1);						// Read back 4 bytes, software version, light and high low bytes of range	
        rbuf[2]=sbuf[0];
        printf("Reg3 : %u\n",rbuf[2]);

        sbuf[0] =0x03;
	    writeData(fd, 1);					// Send these 4 bytes to USBI2C
	    readData(fd,1);						// Read back 4 bytes, software version, light and high low bytes of range	
        rbuf[3]=sbuf[0];
        printf("Reg4 : %u\n",rbuf[3]);

	    closePort(fd);						// Close the port

	    ver = sbuf[0];						
	    range = (rbuf[2] << 8) + rbuf[3];   // Calculate the range
	    printf("Range = %u\n\n", range);
        }

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


