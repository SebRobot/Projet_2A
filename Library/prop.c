//Write by Sebastien Malissard on 1 April 2014

//Motor  max speed 200rpm -> /60 : 3.33 per seconde or 2.84 for 170rpm
//Speed max 1m/s ->127

#include "prop.h"

#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <math.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <unistd.h>


//***Declaration***//
    int idFicI2C; 
    float x_c=0., y_c=0., theta_c=0.;
    float nd1=0., nd2=0.;
    point tabTraj[N]={
        {500.,   0.},
        {500., 500.},
        {  0., 500.},
        {  0.,   0.},
        };



//***Times function***//
    unsigned int micros(){
        struct timeval clock;
        gettimeofday(&clock,NULL);
        return clock.tv_usec + clock.tv_sec*1000000;
        }
    unsigned int millis(){
        struct timeval clock;
        gettimeofday(&clock,NULL);
        return clock.tv_usec/1000 + clock.tv_sec*1000;
        }


//***I2C function***//
    void writeData(int fic, char *buf, int nbytes){
        int bytes;
	
	    if((bytes=write(fic, buf, nbytes)) == -1){					
		    perror("writeData: Error while trying to write data - ");	
	        }
	    else if(bytes != nbytes){
		    printf("Warning : Only %u bytes written out of %u requested\n", bytes, nbytes);	
	        }
        }

    void readData(int fd, char *buf, int nbytes){
        int bytes;

	    if((bytes = read(fd, buf, nbytes)) == -1){
		    perror("readData: Error while trying to read data - ");
	        }
	    else if(bytes != nbytes){
		    printf("Warning : Only %u bytes read out of %u requested\n", bytes, nbytes);
	        }
        }

    int openI2C(void){
        int fd;
				               
        if ((fd = open(FILE_I2C, O_RDWR)) == -1){
	        perror("openPort: Unable to open - ");
            return(-1);
            } 
        else{
            if (ioctl(fd, I2C_SLAVE, ADDR) == -1){
            perror("Failed to acquire bus access and/or talk to slave.");
            return(-1);
                }
            }
        return (fd);
        }

    void closeI2C(int fd)
        {	
	    if(close(fd) == -1){	
		    perror("closePort: Unable to close - ");
	        }
        }


//***Init controller***//
    void InitEncoder(void){
        char bufW[2];
        bufW[0]= COMMAND;
        bufW[1]=0x20;
        writeData(idFicI2C, bufW, 2);
        }

    void InitMode(int mode){
        char bufW[2];
        bufW[0]= MODE;
        bufW[1]=mode;
        writeData(idFicI2C, bufW, 2);
        }

    void InitAcc(int acc){
        char bufW[2];
        bufW[0]= ACCELERATION_RATE;
        bufW[1]=acc;
        writeData(idFicI2C, bufW, 2);
        }


//***Encoder***//
    int degreeFromInit(int mot){ 
        char bufW;
        char bufR[4];
        int d=0;

       if((mot != 1) && (mot != 2)){
            printf("Error : mot must be 1 or 2, current mot is : %d\n", mot);
            return -1;
            }
        if(mot==1){
            bufW = ENC1A;
            }
        else{
            bufW = ENC2A;
            }
										
        writeData(idFicI2C, &bufW, 1);
        readData(idFicI2C, &bufR[0], 4); 
        
        d = bufR[0]<<24;
        d |= bufR[1]<<16;
        d |= bufR[2]<<8;
        d |= bufR[3];

        return d ; //1 tour = 360°
        }

    float dist(int mot){ //0 is dist robot
        int d;

       if((mot != 0) && (mot != 1) && (mot != 2)){
            printf("Error : mot must be 1 or 2, current mot is : %d\n", mot);
            return -1;
            }

        switch(mot){
            case 0 :
                d=(degreeFromInit(1)+degreeFromInit(2))/2;
                break;
            case 1 :
            case 2 :
                d=degreeFromInit(mot);
                break;
            default :
                printf("Imposible error\n");
                return -1;
                break;
            }

        return (-M_PI*RADIUS_WHEEL*d/180.);//in mm
        }


//***Move function direct ***//
    int move(int speed, int angle){ //speed in mm/s between 0 and 1000
        char bufW[2];
        int v, turn;

        if((speed > 1000) || (speed < -1000)){
            printf("Error : speed must be between : [-128 ; 127], current speed is : %d\n", speed);
            return -1;
            }

        if((angle > 90) || (angle < -90)){
            printf("Error : angle must be between : [-180 ; 180], current angle is : %d\n", angle);
            return -1;
            }

        turn=(DIST_WHEELS/2*tan(angle*M_PI/180))*127/1000;
        v=speed*127/1000;

	    bufW[0] = SPEED1;						
        bufW[1] = -((char)-v);			//tan in radian			
        //printf("envoyer a prop1 : %hhi et speed=%d\n",bufW[1],speed);
        writeData(idFicI2C, bufW, 2);	

        bufW[0] = TURN;						
        bufW[1] =((char)turn);					
        //printf("envoyer a prop2 : %hhi et speed=%d\n",bufW[1],turn);		
        writeData(idFicI2C, bufW, 2);	
        }


//***Positioning function***//
//Need a global parameter nd1, nd2 for compare the 3 asser pos

    void pos(float *_x, float *_y, float *_theta){ //optimasation with linear some cas and sin
        float DeltaX, DeltaY, DeltaTheta;
        static float theta=0., x=0., y=0.,d1=0.,d2=0.;

        d1 = nd1 - d1;
        d2 = nd2 - d2;

        DeltaTheta=(d1-d2)/DIST_WHEELS;
        theta += DeltaTheta;   

        DeltaX=(d1+d2)*(d1-d2)/(4.*DIST_WHEELS);
        DeltaY=(d1+d2)/2.;

        x += DeltaX*cos(theta)-DeltaY*sin(theta);
        y += DeltaX*sin(theta)+DeltaY*cos(theta);

        *_x = x;
        *_y = y;
        *_theta = theta;

        d1 = nd1;
        d2 = nd2;
        }

    void pos2(float *_x, float *_y, float *_theta){ //optimasation with linear some cas and sin
        float DeltaX, DeltaY, DeltaTheta;
        static float x=0., y=0.,d1=0.,d2=0., ct = 1., st = 0.;

        d1 = nd1 - d1;
        d2 = nd2 - d2;

        DeltaTheta=(d1-d2)/DIST_WHEELS;
        DeltaX=(d1+d2)*(d1-d2)/(4.*DIST_WHEELS);
        DeltaY=(d1+d2)/2.;

        ct = ct*(1. - DeltaTheta*DeltaTheta/2.) - st*DeltaTheta;
        st = st*(1. - DeltaTheta*DeltaTheta/2.) + ct*DeltaTheta;

        x += DeltaX*ct - DeltaY*st;
        y += DeltaX*st + DeltaY*ct;

        *_x = x;
        *_y = y;
        *_theta = atan2(st, ct);    // normally we need only cos(theta) and sin(theta)

        d1 = nd1;
        d2 = nd2;
        }

    void pos3(float *_x, float *_y, float *_theta){ //without optimisation
        float DeltaX, DeltaY, DeltaTheta, r;
        static float theta=0., x=0., y=0.,d1=0.,d2=0.;

        d1 = nd1 - d1;
        d2 = nd2 - d2;


        if(d1==d2){ //For fix illegal operation 
            DeltaTheta=0.;
            DeltaX=0.;
            DeltaY=d1;
            }
        else {
            r=DIST_WHEELS/2*(d1+d2)/(d1-d2);
            DeltaTheta=atan((d1-d2)/DIST_WHEELS);
            theta += DeltaTheta;   

            DeltaX=r*(1.-cos(DeltaTheta));
            DeltaY=r*sin(DeltaTheta);
            }

        x += DeltaX*cos(theta)-DeltaY*sin(theta);
        y += DeltaX*sin(theta)+DeltaY*cos(theta);

        *_x = x;
        *_y = y;
        *_theta = theta;

        d1 = nd1;
        d2 = nd2;
        }

//***Rotation on the spot***//
    void rot3(float theta){ //Warning : rot is a block function
        float d1, d2, d=0, r, theta_start;
        
        printf("theta ask =%f, ",theta);
        if((theta > 180) || (theta < -180)){
            printf("Warning : theta is not between : [-180 ; 180], current speed is : %f\n", theta);
            theta=fmodf(theta, 360);
            if(theta<-180) theta+=360;
            if(theta> 180) theta-=360;
            }
        printf("theta after correc=%f\n",theta);
        //getchar();

        theta*=M_PI/180;
        theta_start=theta_c;

        while (fabs(theta_c-theta_start)<fabs(theta)){
            nd1 = dist(1);
            nd2 = dist(2);
            pos3(&x_c, &y_c, &theta_c);
            if(theta>0)move(0, 45); //Arbitrary value
            else move(0, -45);
            }
        move(0,0);
        }
/*  void rot(int speed, int theta){ //rot without asser
        float time;
        struct timeval temp, temp2;

        time = DIST_WHEELS/(2*speed)*tan(theta);

        gettimeofday(&temp,NULL);
        gettimeofday(&temp2,NULL);

        printf("time=%ld\n",temp.tv_usec);

        while((temp2.tv_usec-temp.tv_usec)<(time*1000000)){
            move(0,1000/127*speed*2/(DIST_WHEELS*tan(theta*M_PI/180)));
            gettimeofday(&temp2,NULL);
            }
        move(0,0);
        }

    void rot2(float theta){ //FIXME correction différence roue gauche et droite
        float d1, d2, d=0, r;
        printf("Debut rot2 theta=%f\n",theta);
        r=M_PI*DIST_WHEELS*theta/360;

        while (fabs(r)>fabs(d)){
            d1=dist(1);
            d2=dist(2);
            if(theta<0){
                r=-r;
                d=(d2-d1)/2;
                move(0, -45);
                }
            else{
                d=(d1-d2)/2;
                move(0, 45);
                }

            printf("d1=%f, d2=%f, d=%f, r=%f, theta=%f\n", d1, d2, d, r, theta); 
            }
        move(0,0);
        }

*/


//***Programing trajectory***//
    float trajCorr(float x, float y, float theta, float xf, float yf){
        float alpha;

        alpha=atan2(xf-x,yf-y);

        return -alpha-theta;
        }

    int followTraj(point pt){ //Warning is a block function
        float alpha, theta, beta, d;
        int time_prev=0;

        //for start with good orientation
        nd1 = dist(1);
        nd2 = dist(2);
        pos3(&x_c, &y_c, &theta_c);
        alpha=atan2(pt.x-x_c, pt.y-y_c);
        beta=-alpha-theta_c;
        rot3(beta*180/M_PI);

        while(1){
            nd1 = dist(1);
            nd2 = dist(2);
            pos3(&x_c, &y_c, &theta_c);

            beta=trajCorr(x_c, y_c, theta_c, pt.x, pt.y);
            beta=fmodf(beta,2*M_PI);
            theta_c=fmodf(theta_c, 2*M_PI);

            beta=4*beta*180./M_PI;
            if(beta>45) beta=45;
            if(beta<-45) beta=-45;
            if((millis()-time_prev)>100){
                time_prev=millis();
                printf("x_c=%f, y_c=%f, theta_c=%f, beta=%f, x_obj=%f, y_obj=%f\n",x_c,y_c,theta_c,beta,pt.x,pt.y);
                }
            move(SPEED, beta);

            if(((fabs(x_c-pt.x)<1.) && (fabs(y_c-pt.y)<PRES))) //FIXME if the robot can not come
                {
                move(0,0);
                return 1;
                }
            }
        }


/*

int main(){
    unsigned int us_start, t0, t1, t2, t3;
    float x, y, theta;
    float x2, y2, theta2;
    float x3, y3, theta3;
    float beta;
    point pt;
    int i=0;

    idFicI2C=openI2C();

    InitMode(3);    
    InitEncoder();
    InitAcc(2);




while(1){
    followTraj(tabTraj[i]);
    if(i==(N-1))i=0;
    else i++;
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
   /* move(1000,0,idFicI2C);



    sleep(1);




    move(0,0,idFicI2C);
    sleep(1);
    printf("d=%f\n",dist(0));


    close(idFicI2C);
    printf("Fermeture du bus i2c\n");
    
    return 0;
    }

*/
