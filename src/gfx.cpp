#include "gfx.h"

Arduino_DataBus *bus = create_default_Arduino_DataBus();

/* More display class: https://github.com/moononournation/Arduino_GFX/wiki/Display-Class */
Arduino_GFX *gfx = new Arduino_GC9A01(bus, 7 /* RST */, 3 /* rotation */, true /* IPS */);

/*******************************************************************************
 * End of setup
 **************** */


 int16_t w, h, center;


 uint8_t conv2d(const char *p)
{
    uint8_t v = 0;
    return (10 * (*p - '0')) + (*++p - '0');
}


void drawBitmapScale(int16_t x, int16_t y,
                             const uint8_t bitmap[], int16_t w, int16_t h,
                             uint16_t color, uint8_t scale)
{
  int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
  uint8_t byte = 0;
  for (int16_t j = 0; j < h; j++, y+= scale)
  {
    for (int16_t i = 0; i < w; i++)
    {
      if (i & 7)
      {
        byte <<= 1;
      }
      else
      {
        byte = pgm_read_byte(&bitmap[j * byteWidth + i / 8]);
      }
      if (byte & 0x80)
      {
        gfx->fillRect(x + i*scale, y,scale,scale, color);
        
      }
    }
  }
}