// Wire Slave Receiver
// by Nicholas Zambetti <http://www.zambetti.com>
// modified by Sébastien Malissard github : SebRobot

// Demonstrates use of the Wire library
// Receives data as an I2C/TWI slave device
// Refer to the "Wire Master Writer" example for use with this

// Created 29 March 2006 and  modified 22 March 2014

// This example code is in the public domain.

#include <Wire.h>

#define ADDR 0x0a   //address of Arduino 
#define MODE 0      //0 : receive a charactere, 1 : send a charactere

void setup()
  {
  Wire.begin(ADDR);               // join i2c bus with address : ADDR
  Wire.onReceive(receiveEvent);   // register event -> slave mode
  Serial.begin(9600);             // start serial for output
  }

void loop()
  {
  delay(100);
  }

void receiveEvent(int howMany)
  {
  int k;
  char c;
  switch(MODE)
    {
    case(0) :
      if((k=Wire.available())==1) //return the numbers of bytes available
        {
        c=Wire.read();            // receive byte as a character
        Serial.println("Character receive :");
        Serial.println(c);    
        }
      break;
    case(1) :
      break;
    default :
      break;
    }
  }
