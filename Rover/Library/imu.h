#ifndef IMU_H
#define IMU_H

#define ADDR_I2C_IMU 0x69


// Constant
	#define MPU_GYRO_OFF_NB_ITER  50
	#define MPU_ACCEL_GAIN             1        //6.103515625e-05
	#define MPU_GYRO_GAIN              1        //0.030487804878049
	#define MPU_MAGN_GAIN              1        //0.3001221001221001

// Registrer fo Gyroscope and Accelerometer
	#define MPU_SMPRT_DIV       0x19    // Gyro sampling rate divider
	#define MPU_CONFIG          0x1A    // Gyro and accel confi
	#define MPU_GYRO_CONFIG     0x1B    // Gyro configuration
	#define MPU_ACCEL_CONFIG    0x1C    // Accel configuration
	#define MPU_FIFO_EN         0x23    // FIFO buffer control
	#define MPU_INT_PIN_CFG     0x37    // Bypass enable config
	#define MPU_INT_ENABLE      0x38    // Interrupt control
	#define MPU_ACCEL_XOUT_H    0x3B    // Accel X axis High
	#define MPU_ACCEL_XOUT_L    0x3C    // Accel X axis Low
	#define MPU_ACCEL_YOUT_H    0x3D    // Accel Y axis High
	#define MPU_ACCEL_YOUT_L    0x3E    // Accel Y axis Low
	#define MPU_ACCEL_ZOUT_H    0x3F    // Accel Z axis High
	#define MPU_ACCEL_ZOUT_L    0x40    // Accel Z axis Low
	#define MPU_TEMP_OUT_H      0x41    // Temp Hight
	#define MPU_TEMP_OUT_L      0x42    // Temp Low
	#define MPU_GYRO_XOUT_H     0x43    // Gyro X axis High
	#define MPU_GYRO_XOUT_L     0x44    // Gyro X axis Low
	#define MPU_GYRO_YOUT_H     0x45    // Gyro Y axis High
	#define MPU_GYRO_YOUT_L     0x46    // Gyro Y axis Low
	#define MPU_GYRO_ZOUT_H     0x47    // Gyro Z axis High
	#define MPU_GYRO_ZOUT_L     0x48    // Gyro Z axis Low
	#define MPU_USER_CTRL       0x6A    // User control
	#define MPU_PWR_MGMT_1      0x6B    // Power management 1



// Register for Magnetometer
	#define MPU_AKM             0x48	// Fixed value describing device
	#define MPU_MAGN_ADDRESS    0x0C    // Address of magn in bypass mode
	#define MPU_WIA        		0x00	// Magn Who I Am
	#define MPU_INFO      		0x01	// Information
	#define MPU_ST1        		0x02	// Data status 1, data ready
	#define MPU_HXL        		0x03	// X-axis data, lower bit
	#define MPU_HXH        		0x04	// X-axis data, highter bit
	#define MPU_HYL        		0x05	// Y-axis data, lower bit
	#define MPU_HYH        		0x06	// Y-axis data, highter bit
	#define MPU_HZL        		0x07	// Z-axis data, lower bit
	#define MPU_HZH        		0x08	// Z-axis data, highter bit
	#define MPU_ST2        		0x09	// Data status 2
	#define MPU_CNTL       		0x0A	// Control mode
	#define MPU_RSV        		0x0B    // Reserved
	#define MPU_ASTC       		0x0C	// Self-test
	#define MPU_TS1        		0x0D	// Test 1
	#define MPU_TS2        		0x0E	// Test 2	
	#define MPU_I2CDIS     		0x0f	// I2C disable
	#define MPU_ASAX       		0x10	// X-axis sensibility
	#define MPU_ASAY       		0x11	// Y-axis sensibility
	#define MPU_ASAZ       		0x12	// Z-axis sensibility
	#define MPU_9150_AKM_ID     0x48    // Mag device ID
	


// Declaration of funtions
	void mpu_init(void);
//	void mpu_end(void);
	void mpu_read(double* tab_a, double* tab_g, double* tab_m);


#endif // IMU_H
