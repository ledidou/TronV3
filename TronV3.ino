

#include "joystick.h"

// #include "lightcycle.h"
#include "board.h"
#include "display.h"

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

/*
const uint8_t WIDTH = 4; // 53;
const uint8_t HEIGHT = 4; // 40;
const uint8_t STEP = 10; // 3;
*/
const uint8_t WIDTH  = 53;
const uint8_t HEIGHT = 40;
const uint8_t STEP   = 3;

Board<WIDTH,HEIGHT> * board;
Display * display;
Joystick joy;

int x, y;

void setup() {
    Serial.begin(9600);

    Serial.println("Init");

    display = new Display(WIDTH,HEIGHT,STEP);

    board = new Board<WIDTH,HEIGHT>(display);

    x = WIDTH/2;
    y = HEIGHT/2;
    
    board->SetPoint(x,y,ST7735_RED);
    /*
    BitTable<1,4,1,4,1> debug;
    debug.print("Debug");

    board->_point.print("_point");
    */
}

Direction dir;
bool mv = false;

void loop() {  
  joy.Check();
  
  if (joy.left())  {
    dir = West;
    mv = true;
  }
  
  if (joy.right()) {
    dir = East;
    mv = true;
  }
  
  if (joy.up()) {
    dir = North;
    mv = true;
  }

  if (joy.down()) {
    dir = South;
    mv = true;
  }

  if (joy.press()) { 
    mv = false;
  }
  
  // if (joy.neutral()) Serial.println("neutral");
  
  if (mv)
   { 
     if (board->CanMove(x,y,dir)) 
     {
       board->SetSegment(x,y,dir,ST7735_RED);
       move(x,y,dir);
       delay(100);
     }
     else 
     {
       Serial.print( "BOUM(" );
       Serial.print( x );
       Serial.print( "," );
       Serial.print( y );
       Serial.println( ")" );
       //board->print();
       
       board->ClearLine(x,y);
       
       x = WIDTH/2;
       y = HEIGHT/2;
       board->SetPoint(x,y,ST7735_RED);

       mv =false;
     }
   }
}
