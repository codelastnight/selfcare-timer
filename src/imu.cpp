#include "imu.h"


Adafruit_ICM20948 icm;


void imu_init() {
    
  // Try to initialize!
  if (!icm.begin_I2C()) {

    
    while (1) {
      delay(10);
    }
  }
  
}

