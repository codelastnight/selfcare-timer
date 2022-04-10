#include "encoder.h"

int32_t encoder_position;
Adafruit_seesaw ssw;

void ss_init() {
    if (! ssw.begin(SEESAW_ADDR) ) {
        //Serial.println("Couldn't find seesaw on default address");
    while(1) delay(10);
    }
    ssw.pinMode(SS_SWITCH, INPUT_PULLUP);
    encoder_position = ssw.getEncoderPosition();
    delay(10);
    ssw.setGPIOInterrupts((uint32_t)1 << SS_SWITCH, 1);
    ssw.enableEncoderInterrupt();
}

void ss_loop() {

}

