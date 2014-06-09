#ifndef DISPLAY__H
#define DISPLAY__H

#include "Arduino.h"

#include <assert.h>

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

// TFT configuration
#define sclk 13
#define mosi 11
#define cs   10
#define dc   8
#define rst  0  // you can also connect this to the Arduino reset


class Display
{

    Adafruit_ST7735 tft;
    
    static const uint16_t BORDER_COLOR = ST7735_WHITE;
    static const uint16_t BACKGROUND_COLOR = ST7735_BLACK;

    int width, height, step;
    uint8_t x0,y0,x1,y1;

  public :
    Display(int width, int height, int step) : tft(cs, dc, mosi, sclk, rst) {

      tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab

      tft.setRotation(1);
      
      this->width = width;
      this->height = height;
      this->step = step;
      
      this->y1 = tft.height() - 1;
      this->y0 = y1 - step * height;
      this->x0 = 0;
      this->x1 = x0 + step * width;
      assert(y0 >= 0);
      assert(x1 < tft.width());
      
      Clear();
    }
    
    void Clear() {
      tft.fillScreen(BACKGROUND_COLOR);
      tft.drawFastHLine(x0, y0, x1-x0, BORDER_COLOR);
      tft.drawFastHLine(x0, y1, x1-x0, BORDER_COLOR);
      tft.drawFastVLine(x0, y0, y1-y0, BORDER_COLOR);
      tft.drawFastVLine(x1, y0, y1-y0, BORDER_COLOR);
    }
    
    int col2x( int col ) { return x0 + step * col; }
    int line2y( int line ) { return y0 + step * line; }
    
    void DrawPoint(int c, int l, uint16_t color) {
      
      const int x = col2x( c );
      const int y = line2y( l );
    
      tft.drawPixel(x, y, color);

    }
    
    void DrawLine(int c0, int l0, int c1, int l1, uint16_t color) {
      
      const int px0 = col2x( c0 );
      const int py0 = line2y( l0 );
      const int px1 = col2x( c1 );
      const int py1 = line2y( l1 );
      if ( px0 == px1 ) 
        if ( py1 < py0 ) 
          tft.drawFastVLine(px0, py1, py0-py1, color);
        else
          tft.drawFastVLine(px0, py0+1, py1-py0, color);
       else if ( py0 == py1 )
         if ( px1 < px0 ) 
           tft.drawFastHLine(px1, py0, px0-px1, color);
          else
            tft.drawFastHLine(px0+1, py0, px1-px0, color);
        else
          tft.drawLine(px0, py0, px1, py1, color);
       
    }
    
    void ClearLine(int c0, int l0, int c1, int l1) {
      DrawLine(c0, l0, c1, l1, BACKGROUND_COLOR );
    }
    void ClearPoint(int c, int l) {
      DrawPoint(c,l, BACKGROUND_COLOR);
    }
    

};

#endif
