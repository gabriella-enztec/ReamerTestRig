
#include <Bounce2.h>
#include "stepper_movement.h"
#include "ream.h"
#include "comms.cpp"

// Button pin definitions
#define DOWN_BUTTON 2    // Button moves gantry downwards
#define UP_BUTTON 4      // Button moves gantry upwards
#define ZERO_BUTTON 8    // Sets current position as zero
#define REAM_BUTTON 6    // Executes ream cycle

// Button configuration
const int btnPins[] = {DOWN_BUTTON, UP_BUTTON, ZERO_BUTTON, REAM_BUTTON};
const int numberOfButtons = 4;
Bounce btn[numberOfButtons];

// Button state tracking
bool upPressed = false;
bool downPressed = false;


void setup() {
  // Initialize buttons with pull-up resistors
  for (int i = 0; i < numberOfButtons; i++) {
    pinMode(btnPins[i], INPUT_PULLUP);
    btn[i].attach(btnPins[i]);
    btn[i].interval(50);  // 50ms debounce
  }
  
  stepper_movement_init();
  serial_init();
}

void loop() {

  checkSerialCommand();

  // Update all button states
  for (int i = 0; i < numberOfButtons; i++) {
    btn[i].update();
  }

  if(btn[0].fell()) // Down
  {
    downPressed = true;
  }
  else if(btn[0].rose())
  {
    downPressed = false;
  }

  if(btn[1].fell()) // UP
  {
    upPressed = true;
  }
  else if(btn[1].rose())
  {
    upPressed = false;
  }

  manualPositioning(upPressed, downPressed);

  if(btn[2].fell()) // ZERO
  {
    Serial.println(("Zero"));
    zero();
  }

  if(btn[3].fell()) // REAM
  {
    Serial.println("Reaming");
    reamCycle();
  }
}
