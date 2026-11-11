#include <Bounce2.h>
#include "HeaderFiles/buttons.h"
#include "HeaderFiles/ream.h"
#include "HeaderFiles/stepper_movement.h"

// Button configuration - now defined here (not in header)
const int btnPins[] = {DOWN_BUTTON, UP_BUTTON, ZERO_BUTTON, REAM_BUTTON};
const int numberOfButtons = 4;
Bounce btn[numberOfButtons];

// Button state tracking
bool upPressed = false;
bool downPressed = false;

void buttons_init()
{
  for (int i = 0; i < numberOfButtons; i++) 
  {
    pinMode(btnPins[i], INPUT_PULLUP);
    btn[i].attach(btnPins[i]);
    btn[i].interval(50);  // 50ms debounce
  }
}


// Checks physical buttons state
void checkButtons()
{
  // Update all button states
  for (int i = 0; i < numberOfButtons; i++) {
    btn[i].update();
  }

  if(btn[BTN_DOWN].fell()) // Down
  {
    downPressed = true;
  }
  else if(btn[BTN_DOWN].rose())
  {
    downPressed = false;
  }

  if(btn[BTN_UP].fell()) // UP
  {
    upPressed = true;
  }
  else if(btn[BTN_UP].rose())
  {
    upPressed = false;
  }

  manualPositioning(upPressed, downPressed);

  if(btn[BTN_ZERO].fell()) // ZERO
  {
    Serial.println("Zero");
    zero();
  }

  if(btn[BTN_REAM].fell()) // REAM
  {
    Serial.println("Reaming");
    reamCycle();
  }
}