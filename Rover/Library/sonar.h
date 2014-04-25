#ifndef SONAR_H
#define SONAR_H

#define ADDR_I2C_SONAR 0x70

// Register
	#define SONAR_SOFT_REVI  0
	#define SONAR_CMMND      0
	#define SONAR_MAX_GAIN   1
	#define SONAR_RNG_H      2
	#define SONAR_RNG_L      3

// Command
	#define SONAR_RNG_INCH   80
	#define SONAR_RNG_CM     81
	#define SONAR_RNG_US     82
	#define SONAR_CHG_I2C_1  160
	#define SONAR_CHG_I2C_2  165
	#define SONAR_CHG_I2C_3  170


// Declaration of funtions
	int get_sonar(int unit);
	int sonar_get_distance_cm(void);


#endif // SONAR_H
