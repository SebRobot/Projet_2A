
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>

#include "imu.h"
#include "i2c.h"


int idFic = -1;
double  gyro_offset[3] = { 0.0, 0.0, 0.0 };



void mpu_init(void){
	char bufW[6];
	char bufR[6];
	double  tab_g[3]; // tab_a[3], tab_m[3];
    double  offset[3] = { 0.0, 0.0, 0.0 };
    int i;
	
	// Init Accel and Gyro
		// Open I2C an acces to Accel and Gyro 
			idFic = openI2C(ADDR_I2C_IMU);
			printf("idFic mpu_init = %d\n", idFic);
			if(idFic < 0) perror("Error in function ""openI2C()"" to acces accelerometer and gyrometer:");
 			 
		// Fix the sample rate of Gyro to 1kHz
			bufW[0] = MPU_SMPRT_DIV;
			bufW[1] = 0;
			writeData(idFic, bufW, 2);
			
		// Disable FSYNC and fix DLPF at  184 Hz for Accel and 188 Hz for Gyro
			bufW[0] = MPU_CONFIG;
			bufW[1] = 1;
			writeData(idFic, bufW, 2);
			
		// Fix the scale rang of Gyro at +-1000°/s
			bufW[0] = MPU_GYRO_CONFIG;
			bufW[1] = 16;
			writeData(idFic, bufW, 2);
		
		// Fix the scale range of Accel at +-2g
			bufW[0] = MPU_ACCEL_CONFIG;
			bufW[1] = 0;
			writeData(idFic, bufW, 2);
			
		// Disable FIFOs buffers
			bufW[0] = MPU_FIFO_EN;
			bufW[1] = 0;
			writeData(idFic, bufW, 2);
			
		// Enable Bypass Mode, acces to the auxiliary I2C bus on MPU9150
			bufW[0] = MPU_INT_PIN_CFG;
			bufW[1] = 2;
			writeData(idFic, bufW, 2);
			
		// Disable all interrupt
			bufW[0] = MPU_INT_ENABLE;
			bufW[1] = 0;
			writeData(idFic, bufW, 2);
			
		// Disable FIFO and no I2C slaves
			bufW[0] = MPU_USER_CTRL;
			bufW[1] = 0;
			writeData(idFic, bufW, 2);
			
		// Disable low power mode, sleep and enable internal 8 MHz oscillator
			bufW[0] = MPU_PWR_MGMT_1;
			bufW[1] = 0;
			writeData(idFic, bufW, 2);
			
	// Init Magnetometer
		// Acces to magnetometer 
    		  idFic = openI2C(MPU_MAGN_ADDRESS);
printf("idFic mpu_init acces magn = %d\n", idFic);
			  if(idFic < 0) perror("Error in function ""mpu_init()"" to acces magnetometer:");
		
		// Check for the AKM device ID
			bufW[0] = MPU_WIA;
			writeData(idFic, bufW, 1);
			readData(idFic, bufR, 1);
			if ( bufR[0] < 0 ) perror("Error: read WIA");
			if ( bufR[0] != MPU_AKM ) perror("Error: Magn not found");
			
		// Enable single mesurement mode
			bufW[0] = MPU_CNTL;
			bufW[1] = 1;
			writeData(idFic, bufW, 2);
			
		// Enable single mesurement mode
			bufW[0] = MPU_CNTL;
			bufW[1] = 1;
			writeData(idFic, bufW, 2);
			
		// Give time to initialization
			usleep(100000);
					
	// Test reading of data
//		if ( mpu_read( tab_a, tab_g, tab_m ) ) perror("Error: Reading data while mpu_init");
			
	// Calcul of gyro offsets
		for ( i = 0; i < MPU_GYRO_OFF_NB_ITER; i++ )  {
//				if ( mpu_read( tab_a, tab_g, tab_m ) ) perror("Error: Don't calcul gyro offsets");

		offset[0] += tab_g[0];
		offset[1] += tab_g[1];
		offset[2] += tab_g[2];
		}

	gyro_offset[0] = offset[0] / MPU_GYRO_OFF_NB_ITER;
	gyro_offset[1] = offset[1] / MPU_GYRO_OFF_NB_ITER;
	gyro_offset[2] = offset[2] / MPU_GYRO_OFF_NB_ITER;
	
	// Display gyrometer's offset
		for(i=0; i<3; i++){
			printf("gyro_offset[%d] = %.2lf\n", i, gyro_offset[i]);
		}
		printf("\n");
}




/*
void mpu_end(void){
  
  if ( idFic != -1 )  {
    closeI2C(idFic);
    idFic = -1;
  }
}
*/



void mpu_read(double* tab_a, double* tab_g, double* tab_m){
	int i, u=0;
	static int magn_state = 0;
	uint8_t val_ST1;
	char bufW[6];
	char bufR[6];
	static double last_mes[3] = { 0.0, 0.0, 0.0 };
	static uint8_t  magn_block[6] = { 0, 0, 0, 0, 0, 0 };


	// Test idFic	
	    if ( idFic < 0 ) perror("Bad idFic for function ""mpu_read()""");

	// Read acceleration
		if(tab_a != NULL){
//			printf("ACCEL\n");
			// Init of entries
				for(i=0; i<3; i++) tab_a[i] = 0.0;
		
	  		// Acces to accelerometer and gyrometer
	  		if ( ioctl( idFic, I2C_SLAVE, ADDR_I2C_IMU) < 0 ) 
	  		    perror("Error: device not found while read acceleration");
	  		
	  		// Get hight and low bytes for X, Y and Z axis
	  		bufW[0] = MPU_ACCEL_XOUT_H;
			writeData(idFic, bufW, 1);
			readData(idFic, bufR, 6);	
	  		for(i=0; i<3; i++){
	  			tab_a[i] = (double)((int16_t)( (uint8_t)bufR[u] << 8 | (uint8_t)bufR[u+1] ) * MPU_ACCEL_GAIN );
//	  			printf("\tbufR[%d] = %d \t bufR[%d] = %d\n",
//	  			        u, (int16_t)bufR[u], u+1, (int16_t)bufR[u+1]);
	  			u = u+2;
	  		}
//	  		printf("\n");
		}

	// Read gyroscope
		if(tab_g != NULL){
//			printf("GYRO\n");
			// Init of entries
				for(i=0; i<3; i++) tab_g[i] = 0.0;
			
			// Get hight and low bytes for X, Y and Z axis	
			bufW[0] = MPU_GYRO_XOUT_H;
			writeData(idFic, bufW, 1);
			readData(idFic, bufR, 6);	
	  		for(i=0; i<3; i++){
	  			tab_g[i] = (double)((int16_t)( (uint8_t)bufR[u] << 8 | (uint8_t)bufR[u+1] ) * MPU_ACCEL_GAIN );
//	  			printf("\tbufR[%d] = %d \t bufR[%d] = %d\n",
//	  			        u, (int16_t)bufR[u], u+1, (int16_t)bufR[u+1]);
	  			u = u+2;
	  		}
//	  		printf("\n");
		}
		
	// Acces Magnetometer
		if ( ioctl( idFic, I2C_SLAVE, MPU_MAGN_ADDRESS) < 0 ) perror("Error in function ""mpu_read"" to acces magnetometer ");
    		      
	// Read magnetometer
		if(tab_m != NULL){
//			printf("MAGN\n");
			// Init of entries
				for(i=0; i<3; i++) tab_m[i] = 0.0;
			switch(magn_state){
	   			case 0: // Check if data is ready
					bufW[0] = MPU_ST1;
					writeData(idFic, bufW, 1);
					readData(idFic, bufR,1);
					val_ST1 = bufR[0];
					if ((val_ST1 & 0x01) == 1) magn_state = 1;
					
					// Duplicate last measurements
					tab_m[0] = last_mes[0];
					tab_m[1] = last_mes[1];
					tab_m[2] = last_mes[2];
				
					break;
		
				case 1: // Read X-axis
					for ( i = 0; i < 2; i++ )  {
						bufW[0] = MPU_HXL;
						writeData(idFic, bufW, 1);
						readData(idFic, &bufR[i], 2);
						magn_block[i] = bufR[i];
					}
		  			magn_state = 2;
		  
					// Duplicate last measurements
						tab_m[0] = last_mes[0];
						tab_m[1] = last_mes[1];
						tab_m[2] = last_mes[2];
					break;
		  
				case 2:// Read Y axis
					for ( i = 2; i < 4; i++ )  {
						bufW[0] = MPU_HYL;
						writeData(idFic, bufW, 1);
						readData(idFic, &bufR[i], 2);
						magn_block[i] = bufR[i];
					}
					magn_state = 3;

					// Duplicate last measurements
						tab_m[0] = last_mes[0];
						tab_m[1] = last_mes[1];
						tab_m[2] = last_mes[2];
					break;
		
				case 3:// Read Z axis
					for ( i = 4; i < 6; i++ )  {
						bufW[0] = MPU_HZL;
						writeData(idFic, bufW, 1);
						readData(idFic, &bufR[i], 2);
						magn_block[i] = bufR[i];
					}
					
					// Display informations
					for(i=0; i<6; i=i+2){
//						printf("\tmagn_block[%d] = %d \t magn_block[%d] = %d\n",
//						        i, magn_block[i], i+1, magn_block[i+1]);
					}
					u=0;
					for(i=0; i<3; i++){
			  			tab_m[i] = (double)((uint16_t)( magn_block[u] << 8 | magn_block[u+1] ) * MPU_MAGN_GAIN );
			  			u = u+2;
			  		}

					last_mes[0] = tab_m[0];
					last_mes[1] = tab_m[1];
					last_mes[2] = tab_m[2];
				
					// Re-arm single measurement mode
						bufW[0] = MPU_CNTL;
						bufW[1] = 1;
						writeData(idFic, bufW, 2);

					magn_state = 0;
					break;

				default: magn_state = 0;
			}
//			printf("\n");
		}	
} 



