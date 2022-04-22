#ifndef __IMU_INCLUDED__
#define __IMU_INCLUDED__

#include <Adafruit_ICM20X.h>
#include <Adafruit_ICM20948.h>

extern Adafruit_ICM20948 icm;


void imu_init();
void imu_loop();

#endif