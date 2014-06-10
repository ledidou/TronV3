#ifndef DIRECTION__H
#define DIRECTION__H

enum Direction {
  North,
  West,
  South,
  East 
};


void move( int16_t x0, int16_t y0, Direction dir, int16_t & x, int16_t & y ) 
{
  switch(dir) 
  {
    case North :
      x=x0;
      y=y0-1;
      break;
    case South : 
      x=x0;
      y=y0+1;
      break;
    case East : 
      x=x0+1;
      y=y0;
      break;
    case West : 
      x=x0-1;
      y=y0;
      break;
  }
}


void move( int16_t & x, int16_t & y, Direction dir ) 
{
  move(x,y,dir,x,y);
}

#endif


