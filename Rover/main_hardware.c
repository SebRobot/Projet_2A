//gcc -Wall main_hardware.c Library/i2c.c Library/gpio.c Library/i2c.h Library/prop.c Library/prop.h Library/sonar.c Library/sonar.h Library/gpio.h Library/imu.h Library/imu.c Library/tools.c Library/tools.h Library_com_rover/message.h Library_com_rover/message.c -lm -lpthread -o test



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "Library/prop.h"
#include "Library/sonar.h"
#include "Library/i2c.h"
#include "Library/gpio.h"
#include "Library/imu.h"
#include "Library_com_rover/message.h"


int stop=0; // clnt_sock = -1;
float volt = -1;



/*
void updateLedBat(void){
	static int time1 = 0, state_led_red = 1;
	static int time0 = -1;
	if(time0 < 0) time0 = millis(); 
	if((time1 - time0) > 1000){
		time0 = time1;
	    printf("Battery's tension = %.1lf V\n", volt);
		if(volt > 11.5){
		    writeGPIO(GPIO_50, "high");
		    writeGPIO(GPIO_51, "low");
		}
        else{
		    if(volt > 10.5){
		        writeGPIO(GPIO_50, "high");
		        writeGPIO(GPIO_51, "high");
		    }
		    else{
		    	switch(state_led_red){
		    		case 0 : writeGPIO(GPIO_51, "low"); state_led_red = 1; break;
				    case 1 : writeGPIO(GPIO_51, "high"); state_led_red = 0; break; 
				    default : alert(); perror("Error in funcion ""updateLedBat"" for state_led_red"); break;
				}
				writeGPIO(GPIO_50, "low");
		    }
		}
	}
	time1 = millis();
}



void *threadProp(void *param){
    int i=0;

    InitMode(3);
    InitEncoder();
    InitAcc(2);

    while(1){
    	volt = getBatVolt();
    	pthread_mutex_lock(&mtx_order);
        if(stop == 1 || order == STOP) stopRover();
        else if(order == MVT){
        	pthread_mutex_lock(&mtx_ptTraj);
        	followTraj((point)ptTraj);
        	pthread_mutex_unlock(&mtx_ptTraj);
        }
        pthread_mutex_unlock(&mtx_order);
        else if(stop == 0){
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
//        volt = getBatVolt();
//        updateLedBat();


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
    
void *threadBattery(void* param){
	while(1){
		volt = getBatVolt();
    	updateLedBat();
	}
}
*/





void *thread_Com(void *param){
	accept_com();
	printf("Exit thread_Com\n");
	pthread_exit(NULL);
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
    pthread_t /*pthread_prop,
              pthread_gpio,
              pthread_battery,
              */pthread_com;
              
// Lock order and ptTraj
mtx_order = PTHREAD_MUTEX_INITIALIZER;
mtx_ptTraj = PTHREAD_MUTEX_INITIALIZER;


/*    //gpio init
		initGPIO(GPIO_30);
		initGPIO(GPIO_31);
		initGPIO(GPIO_5);
		initGPIO(GPIO_4);
		initGPIO(GPIO_48);
		initGPIO(GPIO_50);
		initGPIO(GPIO_51);
		initGPIO(GPIO_60);
*/

    
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
        
    //Create a thread control gpio
    if(pthread_create(&pthread_battery, NULL, threadBattery, NULL)!=0){
         printf("ERROR; return code from pthread_create()\n");
         getchar();
         exit(-1);
        }
*/
    //Create a thread for  com
    if(pthread_create(&pthread_com, NULL, thread_Com, NULL)!=0){ //&sargThreadSttRover
         printf("ERROR; return code from pthread_create()\n");
         getchar();
         exit(-1);
    }
        
    //Create a thread for send state Rover
    
/*    typedef struct sArgThrdSttCom sArgThrdSttCom;
    struct sArgThrdSttCom{
    	int clt_sockt;
    	sInfos sinf;
    };
    sArgThrdSttCom  sarg_thrd_stt_com;
    sarg_thrd_stt_com.clt_sockt = clnt_sock;
    sarg_thrd_stt_com.sinf = sInfoRover;
    
  
    if(pthread_create(&pthread_state_com, NULL, thread_state_Com, &sarg_thrd_stt_com)!=0){
         printf("ERROR; return code from pthread_create()\n");
         getchar();
         exit(-1);
        }
*/

/*	double tab_acc[3] = {0.0, 0.0, 0.0},
		   tab_gyro[3] = {0.0, 0.0, 0.0},
		   tab_magn[3] = {0.0, 0.0, 0.0};
	int i;
*/

//	mpu_init();

while(1){

/*	mpu_read(tab_acc, tab_gyro, tab_magn);
	printf("\n");
	for(i=0; i<3; i++) printf("tab_acc[%d] = %.2lf\n", i, tab_acc[i]);
	printf("\n");
	for(i=0; i<3; i++) printf("tab_gyro[%d] = %.2lf\n", i, tab_gyro[i]);
	printf("\n");
	for(i=0; i<3; i++) printf("tab_magn[%d] = %.2lf\n", i,  tab_magn[i]);
	printf("\n\n");

	printf("Distance = %d\n\n", sonar_get_distance_cm());
*/	
	// Update Info Rover
	float x = 0, y = 0, theta = 0;
//	pos3(&x, &y, &theta);
//	updateInfoRover(&argThreadSttRover, x, y, theta, getBatVolt(), sonar_get_distance_cm());
	updateInfoRover(&argThreadSttRover, x, y, theta, 10, 40);
	

	
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

/*    close(idFicI2C);
    printf("Fermeture du bus i2c\n");
*/
    return 0;
    }
