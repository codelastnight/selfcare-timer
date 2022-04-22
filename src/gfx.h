#ifndef __GFX_INCLUDED__
#define __GFX_INCLUDED__

#include <Arduino_GFX_Library.h>


#define SIXTIETH 0.016666667
#define TWELFTH 0.08333333
#define SIXTIETH_RADIAN 0.10471976
#define TWELFTH_RADIAN 0.52359878
#define RIGHT_ANGLE_RADIAN 1.5707963
#define DEGREE 0.0174533
#define HALF_RADIAN 3.141595
#define BACKGROUND PINK
#define FORGROUND BLUE


extern Arduino_DataBus *bus;
extern Arduino_GFX *gfx;

extern int16_t w, h, center;

extern uint8_t conv2d(const char *p);

void drawBitmapScale(int16_t x, int16_t y,
                             const uint8_t bitmap[], int16_t w, int16_t h,
                             uint16_t color, uint8_t scale);


#endif // __GFX_INCLUDED__