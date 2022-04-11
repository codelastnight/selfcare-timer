#ifndef __GFX_INCLUDED__
#define __GFX_INCLUDED__

#include <Arduino_GFX_Library.h>


#define SIXTIETH 0.016666667
#define TWELFTH 0.08333333
#define SIXTIETH_RADIAN 0.10471976
#define TWELFTH_RADIAN 0.52359878
#define RIGHT_ANGLE_RADIAN 1.5707963

#define BACKGROUND BLACK
#define MARK_COLOR WHITE
#define SUBMARK_COLOR DARKGREY // LIGHTGREY
#define HOUR_COLOR WHITE
#define MINUTE_COLOR BLUE // LIGHTGREY
#define SECOND_COLOR RED


extern Arduino_DataBus *bus;
extern Arduino_GFX *gfx;

extern int16_t w, h, center;
extern int16_t hHandLen, mHandLen, sHandLen, markLen;
extern float sdeg, mdeg, hdeg;
extern int16_t *cached_points;
extern uint16_t cached_points_idx;
extern int16_t *last_cached_point;
extern uint8_t conv2d(const char *p);

void draw_round_clock_mark(int16_t innerR1, int16_t outerR1, int16_t innerR2, int16_t outerR2, int16_t innerR3, int16_t outerR3);

void draw_and_erase_cached_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t color, int16_t *cache, int16_t cache_len, bool cross_check_second, bool cross_check_hour);




#endif // __GFX_INCLUDED__