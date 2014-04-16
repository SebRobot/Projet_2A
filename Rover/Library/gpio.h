/*
 * gpio.h
 *
 *  Created on: 15 avr. 2014
 *      Author: Sébastien Malissard
 */

#ifndef GPIO_H_
#define GPIO_H_

#define GPIO_30 "30"    //pin 11 in P9
#define GPIO_31 "31"    //pin 13 in P9
#define GPIO_5  "5"     //pin 17 in P9 //FIXME
#define GPIO_4  "4"     //pin 18 in P9 //FIXME
#define GPIO_48 "48"    //pin 15 in P9
#define GPIO_50 "50"    //pin 14 in P9
#define GPIO_51 "51"    //pin 16 in P9
#define GPIO_60 "60"    //pin 12 in P9


//*** Déclaration ***//
int initGPIO(char *gpio);
int writeGPIO(char *gpio, char *state);
int readGPIO(char *gpio);

#endif
