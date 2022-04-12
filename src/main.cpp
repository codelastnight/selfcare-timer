#include "gfx.h"

#include "encoder.h"
#include "sprite.h"
#include <AsyncTimer.h>


#define degreesToRadians(angleDegrees) (angleDegrees * M_PI / 180.0)
#define MAXTIME 600 // in ms
AsyncTimer t;


bool isRunning = false;
static uint32_t ostart,oend;
bool isPressed = false;
static float timer = 0;

void setcircle(int32_t angle, bool reverse = false) {
    float start, end;
   
    uint16_t color ;
    if (reverse) {
        color= BACKGROUND;
        start = angle + 177.0;
        end = angle + 181.0;
    } else {
        color= WHITE;

        start = angle + 179.0 ;
        end = angle + 184.0;
    }
  
    gfx->fillArc(center,center,100,102,ostart,oend,color);

    ostart = start;
    oend = end;
}

void renderClock(int16_t ms) {
    int16_t hh, mm, ss;

    ss = ms / 10;
    mm = ms *100/ 60;
    gfx->setCursor(50, center);
    gfx->setTextSize(4);
    if (ss < 10)  gfx->print("0");
    if (ss < 100)  gfx->print("0");
    gfx->print(ss);
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

    // Draw 60 clock marks
    draw_round_clock_mark(
    // draw_square_clock_mark(
        center - markLen, center,
        center - (markLen * 2 / 3), center,
        center - (markLen / 2), center);

    
    gfx->setTextColor(WHITE,BACKGROUND);
    

}

void loop()
{
    t.handle();
    //run every sec
    t.setInterval([]() {
        
        if (isRunning) {
            //run timer
            gfx->startWrite();
            renderClock(timer);
           
            gfx->endWrite();

        }
          
    }, 1000);

    
    //run every ms
    t.setInterval([]() {
        if (isRunning) {
            timer--;
            float timems = map(timer,0,MAXTIME,0,360);
            gfx->startWrite();

            setcircle(timems, true);
           
            gfx->endWrite();

            if (timer <= 0) {
                ssw.setEncoderPosition(0);
                gfx->fillRect(50,center,7000,70,BACKGROUND);
                timer = 0;
                gfx->fillArc(center,center,100,102,0,360,BACKGROUND);
                isRunning = false;
            }

        } else {
            int32_t new_position = ssw.getEncoderPosition();

            if (encoder_position > new_position) {
                if (new_position > 0) {
                    new_position = 0;
                    ssw.setEncoderPosition(0);

                } 
                float timems = map(-new_position,0,360,0,MAXTIME);

                gfx->startWrite();
                renderClock(timems);
              
                setcircle(-new_position, false);
                gfx->endWrite();
            
                encoder_position = new_position;      // and save for next round

            } else if (encoder_position < new_position) {
                if (new_position < -360) {
                    new_position = -360;
                    ssw.setEncoderPosition(-360);

                }
                float timems = map(-new_position,0,360,0,MAXTIME);

                gfx->startWrite();
                setcircle(-new_position,true);
                renderClock(timems);

                gfx->endWrite();
            
                
                encoder_position = new_position;      // and save for next round
            }

        }
        
     


    }, 100);

    //reset?
    if (! ssw.digitalRead(SS_SWITCH) && !isPressed) {
        //start if not running
        if (!isRunning) {
            if (encoder_position != 0) {
               
                timer = map(-encoder_position,0,360,0,MAXTIME);
                isRunning = true;
            }
           
        } else {
            // else reset timer
            ssw.setEncoderPosition(0);
            gfx->fillRect(50,center,7000,70,BACKGROUND);
            timer = 0;
            gfx->fillArc(center,center,100,102,0,360,BACKGROUND);
            isRunning = false;
        }
        isPressed = true;


    } else if (ssw.digitalRead(SS_SWITCH) && isPressed) {
        isPressed = false;
    }

    

}
