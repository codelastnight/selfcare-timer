#ifndef __ENCODER_INCLUDED__
#define __ENCODER_INCLUDED__

#include "Adafruit_seesaw.h"

#define SS_SWITCH        24
#define SEESAW_ADDR          0x36

extern int32_t encoder_position;
extern Adafruit_seesaw ssw;


void ss_init();
void ss_loop();

#endif