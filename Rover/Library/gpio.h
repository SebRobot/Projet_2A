/*
 * gpio.h
 *
 *  Created on: 15 avr. 2014
 *      Author: Sébastien Malissard
 */

#ifndef GPIO_H_
#define GPIO_H_

//#define GPIO_5        "5"     //pin 18 in P9 //FIXME
//#define GPIO_4        "4"     //pin 17 in P9 //FIXME
#define GREEN_LED     "30"    //pin 11 in P9; green led; gpio 30
#define ORANGE_LED    "31"    //pin 13 in P9; orange led          gpio 31
#define WRNG_RED_LED  "48"    //pin 15 in P9; led ALERT red       gpio 48
#define GREEN_LED_BIC "51"    //pin 14 in P9; led bicolor verte   gpio 51
#define RED_LED_BIC   "50"    //pin 16 in P9; led bicolor rouge   gpio 50
#define BP_STOP       "60"    //pin 12 in P9; BpStop              gpio 60



//*** Déclaration ***//
void alert(void);
int initGPIO(char *gpio);
int writeGPIO(char *gpio, char *state);
int readGPIO(char *gpio);

#endif
