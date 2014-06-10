//gcc -Wall main_hardware.c Library/i2c.c Library/gpio.c Library/i2c.h Library/prop.c Library/prop.h Library/sonar.c Library/sonar.h Library/gpio.h Library/imu.h Library/imu.c Library/tools.c Library/tools.h Library/param.h Library_com_rover/message.h Library_com_rover/message.c ../typeMessage.h -lm -lpthread -o test



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "Library/prop.h"
#include "Library/sonar.h"
#include "Library/i2c.h"
#include "Library/gpio.h"
#include "Library/imu.h"
#include "Library/param.h"
#include "Library_com_rover/message.h"



//int stop = 1; // clnt_sock = -1;
float volt = -1;
int distSonar; // For sonar

pthread_mutex_t mtx_dist_sonar = PTHREAD_MUTEX_INITIALIZER;



void updateLedBat(void){
	int stateLED_Sonar = 0;
	static int time1 = 0, state_led_red = 1;
	static int time0 = -1;
	static int time1Sonar = 0;
	static int time0Sonar = -1;
	
	if(time0 < 0) time0 = millis(); 
	if((time1 - time0) > (SEC_DSPL_TENSION * 1000) ){
		time0 = time1;
	    printf("Battery's tension = %.1lf V\n", volt);
        printf("Sonar = %d\n", distSonar);
		if(volt > MAX_THRESHOLD_TENS){
		    writeGPIO(GREEN_LED_BIC, "high");
		    writeGPIO(RED_LED_BIC, "low");
		}
        else{
		    if(volt > MIN_THRESHOLD_TENS){
		        writeGPIO(RED_LED_BIC, "high");
		        writeGPIO(GREEN_LED_BIC, "high");
		    }
		    else{
		    	switch(state_led_red){
		    		case 0 : writeGPIO(RED_LED_BIC, "low"); state_led_red = 1; break;
				    case 1 : writeGPIO(RED_LED_BIC, "high"); state_led_red = 0; break; 
				    default : alert(); perror("Error in funcion ""updateLedBat"" for state_led_red"); break;
				}
				writeGPIO(GREEN_LED_BIC, "low");
		    }
		}
	}
	time1 = millis();
	
	// Update orange led in function of button STOP
	pthread_mutex_lock(&mtx_order);
	if(order == MVT){
        writeGPIO(ORANGE_LED, "low");
        writeGPIO(GREEN_LED, "high");
	}
    else{
        writeGPIO(ORANGE_LED, "high");
		writeGPIO(GREEN_LED, "low");
    }
    pthread_mutex_unlock(&mtx_order);
    
    // Update orange led in function of sonar
    while(distSonar < DIST_MIN_SONAR){
    	time1Sonar = millis();
    	switch(stateLED_Sonar){
			case 0: writeGPIO(ORANGE_LED, "high");
					if((time1Sonar - time0Sonar) > TIME_BLINK_SONAR){
						stateLED_Sonar = 1;
						time0Sonar = millis();
					}
					break;
			case 1: writeGPIO(ORANGE_LED, "low");
					if((time1Sonar - time0Sonar) > TIME_BLINK_SONAR){
						stateLED_Sonar = 0;
						time0Sonar = millis();
					}
					break;
			default: break;
		}	
    }
}



void *threadProp(void *param){
    int i=0;
    int distSonar;
    static int u = 0;
    #ifdef THREAD_PROP
    //typedef enum {PROP_STP, PROP_MVT_TRAJ, PROP_MVT_TRAJ_POS, PROP_DFLT} eStateProp;
    //eStateProp eSttProp = PROP_STP, eSttProp_prev;
    eSta prevOrder;
    eTypeCmd prevTyp_Cmd = STATE;
    #endif

    InitMode(3);
    InitEncoder();
    InitAcc(2);
    
    order = STP;
    prevOrder = order;

    while(1){
    	volt = getBatVolt();
    	
    	// Update distSonar
    	distSonar = sonar_get_distance_cm();
    	
    	pthread_mutex_lock(&mtx_order);
		pthread_mutex_unlock(&mtx_typ_Cmd);
		
		//printf("order = %s\n"
		//	   "typ_Cmd = %s\n",dspl_eSta(order), dspl_eTypeCmd(typ_Cmd));
		
		#ifdef THREAD_PROP
		if( (order != prevOrder) || (typ_Cmd != prevTyp_Cmd)){
			printf("order = %s\n"
			       "typ_Cmd = %s\n",dspl_eSta(order), dspl_eTypeCmd(typ_Cmd));
			prevOrder = order;
			prevTyp_Cmd = typ_Cmd;
    	}
    	#endif
    	//printf("distSonar = %d, \t DIST_MIN_SONAR = %d\n", distSonar, DIST_MIN_SONAR);
    	
    	// if there is an obstacl
		if(distSonar < DIST_MIN_SONAR) order = STP;
		else order = prevOrder; 
		
        if(order == STP){
            #ifdef THREAD_PROP
		    if(u != 1){
				printf("----Rover stop\n");
			}
			u = 1;
		    #endif
            stopRover();
        }
        else if(order == MVT && typ_Cmd == TRAJ){
            #ifdef THREAD_PROP
		    if(u != 2){
				printf("----Rover is moving to point\n");
			}
			u = 2;
		    #endif
        	pthread_mutex_lock(&mtx_position);
        	if(followTraj((sPt)position.pt) == 1) order = STP;
        	pthread_mutex_unlock(&mtx_position);
        }
        else if(order == MVT && typ_Cmd == POS){
            #ifdef THREAD_PROP
		    if(u != 3){
				printf("----Rover is moving to point and is turnning following angl\n");
			}
			u = 3;
		    #endif
        	pthread_mutex_lock(&mtx_position);
        	if(followTraj((sPt)position.pt) == 1) order = STP;
        	rotOnPt(position.ang);
        	pthread_mutex_unlock(&mtx_position);
        }
        else if(order == MVT && (typ_Cmd != TRAJ || typ_Cmd != POS)){
            #ifdef THREAD_PROP
		    if(u != 4){
				printf("----Rover is following points\n");
			}
			u = 4;
		    #endif
            if(followTraj(tabTraj[i])==1){
                if(i==(N-1))i=0;
                else i++;
            }
        }
        
        pthread_mutex_unlock(&mtx_order);
        pthread_mutex_unlock(&mtx_typ_Cmd);
        
    }
}

void *threadGpio(void *param){
    int etat = 0;
    int bpStop = 0;
    //int time = 0, timePrev = -1;
    
    
	while(1){
		bpStop = readGPIO(BP_STOP);
		//time = millis();
		
		pthread_mutex_lock(&mtx_order);
		switch(etat){ 
		    case 0 : if(bpStop == 0 || order == MVT) etat = 1;
		             order = STP;
		             //if(time - timePrev > 50) timePrev = time;
		             break;
		    case 1 : if(bpStop == 1) etat = 2;
		             order = MVT; 
		             break;
            case 2 : if(bpStop==0 || order == STP) etat=3;
                     order = MVT;
		             //if(time - timePrev > 50) timePrev = time;
                     break;
            case 3 : if(bpStop==1) etat=0;
                     order = STP; 
                     break;
		    default : printf("Error in swicth\n");
		              getchar();
		}
		pthread_mutex_unlock(&mtx_order);
	}
}
    
void *threadBattery(void* param){
	while(1){
		volt = getBatVolt();
    	updateLedBat();
	}
}






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

	// For IMU
/*	double tab_acc[3] = {0.0, 0.0, 0.0},
		   tab_gyro[3] = {0.0, 0.0, 0.0},
		   tab_magn[3] = {0.0, 0.0, 0.0};
	int i;
*/


    pthread_t pthread_prop,
              pthread_gpio,
              pthread_battery,
              pthread_com;
              



    //gpio init
		initGPIO(GREEN_LED);
		initGPIO(ORANGE_LED);
		initGPIO(WRNG_RED_LED);
		initGPIO(RED_LED_BIC);
		initGPIO(GREEN_LED_BIC);
		initGPIO(BP_STOP);


    
    //Create a thread for prop
    idFicI2C=openI2C(FILE_I2C);
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

    //Create a thread for  com
    if(pthread_create(&pthread_com, NULL, thread_Com, NULL)!=0){ //&sargThreadSttRover
         printf("ERROR; return code from pthread_create()\n");
         getchar();
         exit(-1);
    }
        


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
		float x, y, theta, bat;
		#ifdef I2C_OK
		pos3(&x, &y, &theta);
		bat = getBatVolt();
		distSonar = sonar_get_distance_cm();
		#else
		x = 0, y = 0, theta = 0;
		bat = 15;
		dist = 1000;
		#endif
		
		//printf("x = %.2f; y  %.2f; theta = %.2f; bat = %.2f; dist = %d\n", x, y , theta, bat, dist);
		updateInfoRover(&argThreadSttRover, &x, &y, &theta, &bat, &distSonar);
/*		printf("argThreadSttRover.sinf.bat = %d\n"
		       "argThreadSttRover.sinf.son = %.2f\n"
		       "argThreadSttRover.sinf.pos.x = %.2f\n"
		       "argThreadSttRover.sinf.pos.y = %.2f\n"
		       "argThreadSttRover.sinf.ang = %.2f\n\n", (int32_t)argThreadSttRover.sinf.bat, argThreadSttRover.sinf.son, argThreadSttRover.sinf.pos.x, argThreadSttRover.sinf.pos.y, argThreadSttRover.sinf.ang);    
*/
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
