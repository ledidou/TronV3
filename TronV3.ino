

#include "joystick.h"

// #include "lightcycle.h"
#include "board.h"
#include "display.h"

#include "lightcycle.h"

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

#if true
const uint8_t WIDTH = 8; 
const uint8_t HEIGHT = 8; 
const uint8_t STEP = 10; 
#else
const uint8_t WIDTH  = 53;
const uint8_t HEIGHT = 40;
const uint8_t STEP   = 3;
#endif

Board * board; // (WIDTH,HEIGHT);
Display * display;
Joystick joy;
LightCycle * playerLightCycle;

int x, y;

void setup() {
    Serial.begin(9600);

    Serial.println("Init");

    display = new Display(WIDTH,HEIGHT,STEP);

    board = new Board(display,WIDTH,HEIGHT);
    board->print();

    playerLightCycle = new LightCycle(WIDTH*3/4,HEIGHT/2,North, ST7735_RED, board);
}

void loop() {  
  if (playerLightCycle->Alive()) {
    Serial.println( playerLightCycle->Loop() );
    board->print();
    playerLightCycle->Command( joy );
  }
  
  delay(500);
}
