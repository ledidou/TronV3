#include "constants.h"

class LightCycle 
{
  int8_t x,y;
  Direction dir;
  uint16_t color;

  void Trace() 
  {
    Serial.print( x,DEC);
    Serial.print( ',' );
    Serial.println( y,DEC);    
  }  
  
 public :
  
  void Init( int8_t x, int8_t y, Direction dir, uint16_t color ) 
  {
    this->x = x;
    this->y = y;
    this->color = color;
    this->dir = dir;
    
    // TODO tft.drawPixel(x, y, color);

    Trace();
  }
  
  bool Forward() 
  {
    int16_t x0 = x;
    int16_t y0 = y;
    // TODO move( x, y, dir, 2 );
    Trace();

    // tft.drawPixel(x, y, color);
    /* TODO
    if ( x == x0 ) 
      if ( y < y0 ) 
          tft.drawFastVLine(x0, y, y0-y, color);
      else
          tft.drawFastVLine(x0, y0+1, y-y0, color);
    else 
      if ( x < x0 ) 
        tft.drawFastHLine(x, y0, x0-x, color);
      else
        tft.drawFastHLine(x0+1, y0, x-x0, color);
    */  
    return true;
  }

  void Command( Joystick & j ) 
  {
    j.Check();
    switch(dir) {
      case NORTH : 
      case SOUTH : 
        if (j.left()) 
           dir = EAST;
        else if (j.right()) 
           dir = WEST;
        break;
      case EAST : 
      case WEST : 
        if (j.up()) 
            dir = NORTH;
        else if (j.down()) 
            dir = SOUTH;
        break;
    }
  }
};

