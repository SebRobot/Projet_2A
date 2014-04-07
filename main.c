#include "Library/prop.h"





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
