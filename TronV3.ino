

#include "joystick.h"

// #include "lightcycle.h"
#include "board.h"
#include "display.h"

#include "lightcycle.h"

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

const uint8_t WIDTH  = 53;
const uint8_t HEIGHT = 40;
const uint8_t STEP   = 3;

Board   * board;
Display * display;

LightCycle *  playerLightCycle   = 0;
LightCycle ** iaLightCycles      = 0;
int           iaLightCyclesCount = 0;

Joystick * joy;

int alives; // total of ioLightCycles alive

//
// delete light cycles 
//
// all the lines are deleted from screen (faster than clear screen)
//
void freeIaLightCycles() {
  delete playerLightCycle; 
  for( int i =0; i<iaLightCyclesCount; i++) 
    delete iaLightCycles[i];
  delete iaLightCycles;
  iaLightCyclesCount = 0;
}

//
// initialize all light cycles
//
void initIaLightCycles(int count) {
  freeIaLightCycles();
  
  playerLightCycle = new LightCycle(WIDTH*3/4,HEIGHT/2,West, ST7735_RED, board);
  
  iaLightCyclesCount = count;
  iaLightCycles = new LightCycle*[iaLightCyclesCount];
  
  for( int i =0; i<iaLightCyclesCount; i++) {
    int y = HEIGHT / 4 + HEIGHT/2 * i / (iaLightCyclesCount-1); 
    iaLightCycles[i] = new LightCycle(WIDTH*1/4,y,East, ST7735_BLUE, board);
  }
  
  alives = iaLightCyclesCount;
}

void setup() {
    Serial.begin(9600);

    Serial.println("Init");

    display = new Display(WIDTH,HEIGHT,STEP);

    board = new Board(display,WIDTH,HEIGHT);

    joy = new Joystick();

    initIaLightCycles( 1 );
}

void loop() {
  
  if (!playerLightCycle->Alive()) {
    // player lose
    // 0.5 second before restart same number of light cycles
    delay(500);
    // restart
    initIaLightCycles( iaLightCyclesCount );
    return;
  }
  
  if (alives == 0) {
     // player win
     delay(500);
     // restart with one more lightcycle 
     initIaLightCycles( iaLightCyclesCount + 1 );
     delay(500);
     return; 
  }

  // it's a game loop

  //   
  playerLightCycle->Command( joy );
  
  if (playerLightCycle->Loop()) {
  
    alives = 0; // counter of lightcycles 
    for( int i=0; i<iaLightCyclesCount; i++) {
      if (iaLightCycles[i]->Alive()) {
        // call Ia with the player as target 
        iaLightCycles[i]->Ia( playerLightCycle );
        if (iaLightCycles[i]->Loop()) 
          alives++;
      }
    }
  }
  delay(50);
}
