/*
 * prop.h
 *
 *  Created on: 7 avr. 2014
 *      Author: seb
 */

#ifndef PROP_H_
#define PROP_H_

//Programme
#define N       4       //sizeof tabTraj
#define SPEED   200
#define PRES    1.

//I2C
#define ADDR 0x58                    //Address of SFR10 (default : 0xB0 -> 0x58)
#define FILE_I2C "/dev/i2c-1"        // default : /dev/i2c-1 on P9 with 19 : SCL and 20 : SDA

//Mechanical characteristic
#define DIST_WHEELS 212. //in mm
#define RADIUS_WHEEL 50. //in mm

//Register of the MD25 controller
#define SPEED1              0x00
#define SPEED2              0x01
#define TURN                0x01
#define ENC1A               0x02
#define ENC1B               0x03
#define ENC1C               0x04
#define ENC1D               0x05
#define ENC2A               0x06
#define ENC2B               0x07
#define ENC2C               0x08
#define ENC2D               0x09
#define BATTERY_VOLT        0x0a
#define MOTOR_1_CURRENT     0x0b
#define MOTOR_2_CURRENT     0x0c
#define SOFTWARE_REVISION   0x0d
#define ACCELERATION_RATE   0x0e
#define MODE                0x0f
#define COMMAND             0x10


//***Define***//
    typedef struct{
        float x;
        float y;
        }point;

        extern int idFicI2C;
        extern float x_c, y_c, theta_c;
        extern float nd1, nd2;
        extern point tabTraj[N];

#endif /* PROP_H_ */
