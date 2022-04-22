#include "gfx.h"

#include "encoder.h"
#include "sprite.h"
#include "imu.h"

#define MAXTIME 60000 // in ms

#define BUZZ 6
#define D3 147
#define E3 165


unsigned long everySecond;
unsigned long everyTenth;
unsigned long startMillis;

bool isRunning = false;
bool isEnd = false;

static uint32_t ostart,oend;
bool isPressed = false;
static float timer = 0;

void reset(bool end = false) {
    ssw.setEncoderPosition(0);
    gfx->fillRect(50,center,7000,70,BACKGROUND);
    timer = 0;
    gfx->fillArc(center,center,100,102,0,360,BACKGROUND);
    isRunning = false;
    isEnd = end;
    noTone(BUZZ);    
}

void setcircle(int32_t angle, bool reverse = false) {
    float start, end;
   
    uint16_t color ;
    if (reverse) {
        color= BACKGROUND;
        start = angle + 177.0;
        end = angle + 181.0;
    } else {
        color= FORGROUND;

        start = angle + 179.0 ;
        end = angle + 184.0;
    }
  
    gfx->fillArc(center,center,100,105,ostart,oend,color);

    ostart = start;
    oend = end;
}

void renderClock(int16_t ms) {
    int16_t  ss;

    ss = ms / 1000;
    gfx->setCursor(50, center);
    gfx->setTextSize(4);
    if (ss < 10)  gfx->print("0");
    if (ss < 100)  gfx->print("0");
    gfx->print(ss);
}
void setup(void)
{
    pinMode(BUZZ, OUTPUT);
    imu_init();
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
   
    gfx->setTextColor(FORGROUND,BACKGROUND);
    
    everySecond = millis();
    everyTenth = millis();
}
uint8_t charloop = 0;

void idleChar(uint8_t cycle) {
    gfx->startWrite();
    switch (cycle) {
        case 1: 
            drawBitmapScale(100,100 , character2, CH_W, CH_H, BACKGROUND,4);
            drawBitmapScale(100,100 , character, CH_W, CH_H, FORGROUND,4);
            break;
        default: 
            drawBitmapScale(100,100 , character, CH_W, CH_H, BACKGROUND,4);
            drawBitmapScale(100,100 , character2, CH_W, CH_H, FORGROUND,4);
            break;
    }
    gfx->endWrite();

}


void loop()
{
    unsigned long runMillis= millis();
    if(runMillis >= everySecond) {
        everySecond += 500;
        if (isRunning) {
            //run timer
            gfx->startWrite();
            renderClock(timer);
           
            gfx->endWrite();

        }
        
        if (charloop > 1) charloop = 0;
        idleChar(charloop);
        charloop++;


    }
    if(runMillis >= everyTenth) {
        everyTenth += 100;

          if (isRunning) {
            timer = timer - 100;

            float timems = map(timer,0,MAXTIME,0,360);
            gfx->startWrite();

            setcircle(timems, true);
           
            gfx->endWrite();

            if (timer <= 0) {
                reset(true);
            }

        } else if (isEnd && !isRunning) {
            gfx->setCursor(50, center);
            gfx->setTextSize(4);
          
           
            switch (everyTenth % 900) {
                case (100): 
                    tone(BUZZ, D3);                     //play the frequency for c
                     gfx->print("stand");
                break;
                case (500):
                    tone(BUZZ, E3);                     //play the frequency for c
                    gfx->print("     up!");

                break;
                default: 
                    noTone(BUZZ);     

            }
            sensors_event_t accel;
            sensors_event_t gyro;
            sensors_event_t mag;
            sensors_event_t temp;
            icm.getEvent(&accel, &gyro, &temp, &mag);
            // reset if move
            if  (abs(gyro.gyro.y) > 0.7) {
                 reset();
            }


        } 
        else {
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
        
     

     }

    //reset?
    if (! ssw.digitalRead(SS_SWITCH) && !isPressed) {
        //start if not running
        if (!isRunning && !isEnd) {
            if (encoder_position != 0) {
                startMillis = runMillis;
                timer = map(-encoder_position,0,360,0,MAXTIME);
                isRunning = true;
                isEnd = false;
            }

            
           
        } else if (isRunning || isEnd) {
            // else reset timer
            reset();


        }
        isPressed = true;


    } else if (ssw.digitalRead(SS_SWITCH) && isPressed) {
        isPressed = false;
    }

    

}
