#include "sonar.h"
#include "i2c.h"




int init_sonar(int idFic, char unit){
	bufW = unit;
	setI2C(idFic, ADDR_SONAR);
	writeData(idFicI2C, &bufW, 1);

	return 0;
}


int get_sonar(int idFic, int unit){//0=inches; 1=cm; 2=us
	int d;
	setI2C(idFic, ADDR_SONAR);
	readData(idFic, bufR, 2);

	d = ((int)bufR[0] << 8) + (int) bufR[1];

	return d;
}
