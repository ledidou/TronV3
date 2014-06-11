#ifndef LIGHTCYCLE__H
#define LIGHTCYCLE__H

#include "direction.h"
#include "board.h"

#include <random.h>

class LightCycle 
{
  int16_t x,y;
  Direction dir;
  uint16_t color;
  Board * board;
  bool alive;
  
 public :
  
  LightCycle( int16_t x, int16_t y, Direction dir, uint16_t color, Board * board ) 
  {
    this->x = x;
    this->y = y;
    this->color = color;
    this->dir = dir;
    this->board = board;
    
    alive = true;
    
    board->SetPoint(x,y,color);
  }

  ~LightCycle() 
  {
    if (alive)
      board->ClearLine(x,y);
  }
  
  bool Alive() {
    return alive;
  }
  
  bool Loop() 
  {
    if (alive) {

      if (board->CanMove(x,y,dir)) 
      {
        board->SetSegment(x,y,dir,color);
        move(x,y,dir);
      }
      else
      {
        // CRASH !
        board->ClearLine(x,y);
        alive = false;
      }
    }      
    return alive;
  }
  
#if true
  void Command( Joystick * j ) 
  {
    j->Check();
    switch(dir) {
      case North : 
      case South : 
        if (j->left()) {
           dir = West;
        } else if (j->right()) { 
           dir = East;
        }
        break;
      case East : 
      case West : 
        if (j->up()) {
            dir = North;
        } else if (j->down()) {
            dir = South;
        }
        break;
    }
  }
#else
  // for debug
  void Command( Joystick * j ) 
  {
        j->Check();
        alive=true;
        if (j->left()) {
           dir = West;
        } else if (j->right()) { 
           dir = East;
        } else if (j->up()) {
            dir = North;
        } else if (j->down()) {
            dir = South;
        } else 
          alive = false;
  }
#endif

  //
  // IA
  //
  // currently very stupid : 
  //   - turn randomly if wall
  //   - don't care of target !
  //
  void Ia( LightCycle * target ) 
  {
    if (alive) {
      if (board->CanMove(x,y,dir)) 
        // if possible continue in this direction
        return;

      Direction sav_dir = dir;
      
      if (random(2)==1) 
        dir = turnright(dir);
      else  
        dir = turnleft(dir);
  
      // try a direction        
      if (board->CanMove(x,y,dir)) 
        return;
      
      // try the other
      dir = turnback(dir);
      if (board->CanMove(x,y,dir)) 
        return;
        
      // continue if impossible to turn (it's the end !)
      dir = sav_dir;      
    }
  }
};

#endif LIGHTCYCLE__H
