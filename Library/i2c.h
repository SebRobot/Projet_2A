#ifndef I2C_H
#define I2C_H

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>


// path to i2c-1 (P9.19 = SCL; P9.20 = SDA)
#define FILE_I2C "/dev/i2c-1"

// Sensors  and register address
#define ADDR_IMU 0x68




// Id file I2C
int idFicI2C;

void writeData(int fic, char *buf, int nbytes);
void readData(int fd, char *buf, int nbytes);
int setI2C(int addr, int fd);
int openI2C(void);
void closeI2C(int fd);

#endif // I2C_H
