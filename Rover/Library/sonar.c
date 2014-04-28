
#include <unistd.h>

#include "sonar.h"
#include "i2c.h"






int get_sonar( int unit){//0=inches; 1=cm; 2=us
	char bufW[4];
	char bufR[4];
	int d, idFic; // openI2C(ADDR_SONAR),
	
	// Open I2C and acces to sonar
		idFic = openI2C((char*)ADDR_I2C_SONAR);
		if(idFic < 0) perror("Error in function ""openI2C()"":");
			    
	// Init sonar
	    bufW[0] = SONAR_CMMND;						
	    bufW[1] = SONAR_RNG_CM;						
	    writeData(idFic, bufW, 2);					
        usleep(80000); 
	// Read soft version                     
        readData(idFic, &bufR[0], 1);				                    
    // Read unknown
	    bufW[0] = 0x01;						
	    writeData(idFic, bufR, 1);					
        readData(idFic, &bufR[1], 1);						
    // Read hight byte of range
        bufW[0] = SONAR_RNG_H ;
	    writeData(idFic, bufW, 1);					
	    readData(idFic, &bufR[2], 1);						
	// Read low byte of range
        bufW[0] = SONAR_RNG_L;
	    writeData(idFic, bufW, 1);					
	    readData(idFic, &bufR[3], 1);
	    
	// Close I2C				
	closeI2C(idFic);

	// Calcul of range
		d = ((int)bufR[2] << 8) + (int) bufR[3];

/*	// Display informatons
		for(i=0; i<4; i++){
		printf("bufR[%d] = %d\n", i, bufR[i]);
		}
	printf("Range = %d\n", d);
*/
	return d;
}





int sonar_get_distance_cm(void){
	char bufW[4] = {'0', '0', '0', '0'};
	char bufR[4] = {'0', '0', '0', '0'};
	int d, idFic;

	// Open I2C and acces to sonar
		idFic = openI2C((char*)ADDR_I2C_SONAR);
		if(idFic < 0) perror("Error in function ""openI2C()"":");

	// Init sonar
	    bufW[0] = SONAR_CMMND;						
	    bufW[1] = SONAR_RNG_CM;						
	    writeData(idFic, bufW, 2);					
        usleep(80000); 
	// Read soft version                     
        readData(idFic, &bufR[0], 1);				                    
    // Read unknown
	    bufW[0] = 0x01;						
	    writeData(idFic, bufR, 1);					
        readData(idFic, &bufR[1], 1);						

    // Read hight byte of range
        bufW[0] = SONAR_RNG_H ;
	    writeData(idFic, bufW, 1);					
	    readData(idFic, &bufR[2], 1);						
	// Read low byte of range
        bufW[0] = SONAR_RNG_L;
	    writeData(idFic, bufW, 1);					
	    readData(idFic, &bufR[3], 1);
	    
	// Close I2C				
	closeI2C(idFic);

	// Calcul of range
		d = ((int)bufR[2] << 8) + (int) bufR[3];

/*	// Display informatons
		for(i=2; i<4; i++){
		printf("bufR[%d] = %d\n", i, bufR[i]);
		}
	printf("Distance = %d\n", d);
*/
	return d;
}
