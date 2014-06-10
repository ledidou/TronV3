#ifndef LIGHTCYCLE__H
#define LIGHTCYCLE__H

#include "direction.h"
#include "board.h"

class LightCycle 
{
  int16_t x,y;
  Direction dir;
  uint16_t color;
  Board * board;
  bool alive;
  bool mv; 
  void Trace() 
  {
    Serial.print( x,DEC);
    Serial.print( ',' );
    Serial.println( y,DEC);    
  }  
  
 public :
  
  LightCycle( int16_t x, int16_t y, Direction dir, uint16_t color, Board * board ) 
  {
    this->x = x;
    this->y = y;
    this->color = color;
    this->dir = dir;
    this->board = board;
    
    alive = true;
    
    mv = false; // set to true for debug
    
    board->SetPoint(x,y,color);
  }
  
  bool Alive() {
    return alive;
  }
  
  bool Loop() 
  {
    Serial.println("LOOP");
    if (alive and mv) {

      if (board->CanMove(x,y,dir)) 
      {
        Serial.println("MOVE");
        board->SetSegment(x,y,dir,color);
        move(x,y,dir);
      }
      else
      {
        Serial.println("BOOM");
        board->ClearLine(x,y);
        alive = false;
      }
    }      
    return alive;
  }
#ifdef false
  void Command( Joystick & j ) 
  {
    j.Check();
    switch(dir) {
      case North : 
      case South : 
        if (j.left()) {
           dir = West;
        } else if (j.right()) { 
           dir = East;
        }
        break;
      case East : 
      case West : 
        if (j.up()) {
            dir = North;
        } else if (j.down()) {
            dir = South;
        }
        break;
    }
    mv = true;
    Serial.println(dir);
  }
#else
  void Command( Joystick & j ) 
  {
    j.Check();
    mv = true;
        if (j.left()) {
           dir = West;
        } else if (j.right()) { 
           dir = East;
        } else if (j.up()) {
            dir = North;
        } else if (j.down()) {
            dir = South;
        } else 
          mv = false;
    if (mv)
      Serial.print("Move");
    Serial.println(dir);
  }
#endif
};

#endif LIGHTCYCLE__H
