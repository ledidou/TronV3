#include "Arduino.h"

class Joystick {
  public :
    enum State {
      Neutral = 0,
      Press   = 5,
      Right   = 1,
      Left    = 2,
      Down    = 3,
      Up      = 4
    };
  private :
    State state;  
  public :
    State Check() {
      int analog = analogRead(3);
  
      if (analog < 50) 
        state = Right;
      else if (analog < 150)   
        state = Up;
      else if (analog < 250)   
        state = Press;
      else if (analog < 500)
        state = Left;
      else if (analog < 650)  
        state = Down;
      else 
        state = Neutral;
      return state;
    };
    bool left()     { return state == Left;    }
    bool right()    { return state == Right;   }
    bool up()       { return state == Up;      }
    bool down()     { return state == Down;    }
    bool press()    { return state == Press;   }
    bool neutral()  { return state == Neutral; }
};

