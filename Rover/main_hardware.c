//gcc -Wall main_hardware.c Library/i2c.c Library/gpio.c Library/i2c.h Library/prop.c Library/prop.h Library/sonar.c Library/sonar.h Library/gpio.h Library/imu.h Library/imu.c -lm -lpthread -o test


#include "Library/prop.h"
#include "Library/sonar.h"
#include "Library/i2c.h"
#include "Library/gpio.h"
#include "Library/imu.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int stop=0;
float volt;

void updateLedBat(void){

    //printf("tension=%f\n", volt);

    if(volt > 11.5){
        writeGPIO(GPIO_50, "low");
        writeGPIO(GPIO_51, "high");
        }
    else{
        if(volt > 11){
            writeGPIO(GPIO_50, "high");
            writeGPIO(GPIO_51, "high");
            }
        else{
            writeGPIO(GPIO_50, "high");
            writeGPIO(GPIO_51, "low");
            }
        }
    }

void *threadProp(void *param){
    int i=0;

    InitMode(3);
    InitEncoder();
    InitAcc(2);

    while(1){
    volt = getBatVolt();
            if(stop==1) stopRover();
            else{
                if(followTraj(tabTraj[i])==1){
                    if(i==(N-1))i=0;
                    else i++;
                }
            }
        }
    }

void *threadGpio(void *param){
    int etat=0;
    int bpstop=0;

    while(1){
        bpstop=readGPIO(GPIO_60);
        updateLedBat();

        switch(etat){ 
            case 0 : if(bpstop==0) etat=1;
                    stop=0;
                    break;
            case 1 : if(bpstop==1) etat=2;
                    stop=1; 
                    break;
            case 2 : if(bpstop==0) etat=3;
                    stop=1;
                    break;
            case 3 : if(bpstop==1) etat=0;
                    stop=0; 
                    break;
            default : printf("Error in swicth\n");
                        getchar();

            }

        if(stop==0)
        writeGPIO(GPIO_48, "low");
        else
        writeGPIO(GPIO_48, "high");

        }
    }
    




int main(){
//    unsigned int us_start, t0, t1, t2, t3;
//    float x, y, theta;
//    float x2, y2, theta2;
//    float x3, y3, theta3;
//    float beta;
//    point pt;
//    int i=0;
//    float check;
    pthread_t pthread_prop, pthread_gpio;


    //gpio init
    initGPIO(GPIO_30);
    initGPIO(GPIO_31);
    initGPIO(GPIO_5);
    initGPIO(GPIO_4);
    initGPIO(GPIO_48);
    initGPIO(GPIO_50);
    initGPIO(GPIO_51);
    initGPIO(GPIO_60);

/*    //Create a thread for prop
    idFicI2C=openI2C();
    setI2C(ADDR_MD25, idFicI2C);
    if(pthread_create(&pthread_prop, NULL, threadProp, NULL)!=0){
         printf("ERROR; return code from pthread_create()\n");
         getchar();
         exit(-1);
        }

    //Create a thread control gpio
    if(pthread_create(&pthread_gpio, NULL, threadGpio, NULL)!=0){
         printf("ERROR; return code from pthread_create()\n");
         getchar();
         exit(-1);
        }
*/

	double tab_acc[3] = {0.0, 0.0, 0.0},
		   tab_gyro[3] = {0.0, 0.0, 0.0},
		   tab_magn[3] = {0.0, 0.0, 0.0};
	int i;


	mpu_init();

while(1){
	mpu_read(tab_acc, tab_gyro, tab_magn);
	printf("\n");
	for(i=0; i<3; i++) printf("tab_acc[%d] = %.2lf\n", i, tab_acc[i]);
	printf("\n");
	for(i=0; i<3; i++) printf("tab_gyro[%d] = %.2lf\n", i, tab_gyro[i]);
	printf("\n");
	for(i=0; i<3; i++) printf("tab_magn[%d] = %.2lf\n", i,  tab_magn[i]);
	printf("\n\n");

	printf("Distance = %d\n\n", sonar_get_distance_cm());
}





//old test
/* //trajectoire
    while(1){
        usleep(10000);
        nd1 = dist(1);
        nd2 = dist(2);
        pos3(&x3, &y3, &theta3);

        beta=trajCorr(x3, y3, theta3, 500, 500);
        printf("%.2f, %.2f, %.2f°, beta=%f\n", x3, y3, theta3, beta*180./M_PI);
        move(100, beta*180./M_PI);
        if(((fabs(x3-500.)<10.) && (fabs(y3-500.)<10.)))
            {
            move(0,0);
            printf("fin\n");
            return 1;
            }
        }
*/




/*
        while(1){
        usleep(10000);
    us_start = micros();
        nd1 = dist(1);
    t0 = micros() - us_start;
        nd2 = dist(2);

    us_start = micros();
        pos(&x, &y, &theta);
    t1 = micros() - us_start;

    us_start = micros();
        pos2(&x2, &y2, &theta2);
    t2 = micros() - us_start;

    us_start = micros();
        pos3(&x3, &y3, &theta3);
    t3 = micros() - us_start;

    //printf("%u,%u,%u,%u\n", t0, t1, t2, t3);

    printf("%.2f, %.2f, %.2f°, %.2f, %.2f, %.2f°, %.2f, %.2f, %.2f°\n", x, y, theta*180./M_PI, x2, y2, theta2*180./M_PI, x3, y3, theta3*180./M_PI);

    }
   // move(1000,0,idFicI2C);



    sleep(1);




    move(0,0,idFicI2C);
    sleep(1);
    printf("d=%f\n",dist(0));
*/

    close(idFicI2C);
    printf("Fermeture du bus i2c\n");

    return 0;
    }
