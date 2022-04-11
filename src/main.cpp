#include "gfx.h"

#include "encoder.h"
#include "sprite.h"


static int16_t osx = 0, osy = 0, omx = 0, omy = 0, ohx = 0, ohy = 0; // Saved H, M, S x & y coords
static int16_t nsx, nsy, nmx, nmy, nhx, nhy;                         // H, M, S x & y coords
static int16_t xMin, yMin, xMax, yMax;                               // redraw range
static int16_t hh, mm, ss;
static unsigned long targetTime; // next action time


static int16_t oldDeg;

void redraw_hands_cached_draw_and_erase()
{
    gfx->startWrite();
    draw_and_erase_cached_line(center, center, nsx, nsy, SECOND_COLOR, cached_points, sHandLen + 1, false, false);

    draw_and_erase_cached_line(center, center, nhx, nhy, HOUR_COLOR, cached_points + ((sHandLen + 1) * 2), hHandLen + 1, true, false);
    draw_and_erase_cached_line(center, center, nmx, nmy, MINUTE_COLOR, cached_points + ((sHandLen + 1 + hHandLen + 1) * 2), mHandLen + 1, true, true);
    gfx->endWrite();
}


void setup(void)
{
    ss_init ();
    gfx->begin();
    gfx->fillScreen(BACKGROUND);

#ifdef DF_GFX_BL
    pinMode(DF_GFX_BL, OUTPUT);
    digitalWrite(DF_GFX_BL, HIGH);
#endif

    // init LCD constant
    w = gfx->width();
    h = gfx->height();
    if (w < h)
    {
        center = w / 2;
    }
    else
    {
        center = h / 2;
    }
    hHandLen = center * 3 / 8;
    mHandLen = center * 2 / 3;
    sHandLen = center * 5 / 6;
    markLen = sHandLen / 6;
    cached_points = (int16_t *)malloc((hHandLen + 1 + mHandLen + 1 + sHandLen + 1) * 2 * 2);

    // Draw 60 clock marks
    draw_round_clock_mark(
    // draw_square_clock_mark(
        center - markLen, center,
        center - (markLen * 2 / 3), center,
        center - (markLen / 2), center);

    hh = conv2d(__TIME__);
    mm = conv2d(__TIME__ + 3);
    ss = conv2d(__TIME__ + 6);

    targetTime = ((millis() / 1000) + 1) * 1000;
    
}

void loop()
{

 
    unsigned long cur_millis = millis();
    if (cur_millis >= targetTime)
    {
        targetTime += 1000;
        ss++; // Advance second
        if (ss == 60)
        {
            ss = 0;
            mm++; // Advance minute
            if (mm > 59)
            {
                mm = 0;
                hh++; // Advance hour
                if (hh > 23)
                {
                    hh = 0;
                }
            }
        }
    }

    // Pre-compute hand degrees, x & y coords for a fast screen update
    sdeg = SIXTIETH_RADIAN * ((0.001 * (cur_millis % 1000)) + ss); // 0-59 (includes millis)
    nsx = cos(sdeg - RIGHT_ANGLE_RADIAN) * sHandLen + center;
    nsy = sin(sdeg - RIGHT_ANGLE_RADIAN) * sHandLen + center;
    if ((nsx != osx) || (nsy != osy))
    {
        mdeg = (SIXTIETH * sdeg) + (SIXTIETH_RADIAN * mm); // 0-59 (includes seconds)
        hdeg = (TWELFTH * mdeg) + (TWELFTH_RADIAN * hh);   // 0-11 (includes minutes)
        mdeg -= RIGHT_ANGLE_RADIAN;
        hdeg -= RIGHT_ANGLE_RADIAN;
        nmx = cos(mdeg) * mHandLen + center;
        nmy = sin(mdeg) * mHandLen + center;
        nhx = cos(hdeg) * hHandLen + center;
        nhy = sin(hdeg) * hHandLen + center;

        // redraw hands
        redraw_hands_cached_draw_and_erase();

        ohx = nhx;
        ohy = nhy;
        omx = nmx;
        omy = nmy;
        osx = nsx;
        osy = nsy;

        delay(1);
    }

    if (! ssw.digitalRead(SS_SWITCH)) {
        ssw.setEncoderPosition(0);
        gfx->fillRect(50,50,50,10,BACKGROUND);
        oldDeg = 0;
    }

    int32_t new_position = ssw.getEncoderPosition();

    // did we move arounde?
    if (encoder_position > new_position) {

        gfx->setCursor(50, 50);
        gfx->setTextColor(WHITE,BLACK);
        if (new_position < 0) new_position = 0;

        if (new_position<10 && new_position > -1) gfx->print( "0");
        gfx->println( new_position);
        // int16_t deg = map(new_position,0,200,0,359) ;
        // int16_t diff = abs(deg - oldDeg);
        // gfx->startWrite();

        // gfx->fillArc(center,center,center - markLen, center, deg,deg + diff,WHITE);

        // gfx->fillArc(center,center,center - markLen, center, oldDeg,oldDeg +diff, BLACK);
        // oldDeg = deg;
        // encoder_position = new_position;      // and save for next round
        // gfx->endWrite();

    } else if (encoder_position < new_position) {
        gfx->setCursor(50, 50);
        gfx->setTextColor(WHITE,BLACK);
                if (new_position < 0) new_position = 0;

        if (new_position<10 && new_position > -1) gfx->print( "0");

         gfx->println( new_position);
    //     int16_t deg = map(new_position,0,200,0,359) ;
    //     int16_t diff = abs(deg - oldDeg);
    //     gfx->startWrite();

    //    gfx->fillArc(center,center,center - markLen, center, oldDeg- diff,oldDeg ,BLACK);
    //     gfx->fillArc(center,center,center - markLen, center,deg - diff,deg ,WHITE);
    //             gfx->endWrite();

    //     oldDeg = deg;
    //     encoder_position = new_position; 
    }

}
