#include <Bounce2.h>
#include "buttons.h"
#include "ream.h"
#include "stepper_movement.h"
#include "comms.h"
#include "settings.h"

// Button configuration - now defined here (not in header)
const int btnPins[] = {DOWN_BUTTON, UP_BUTTON, ZERO_BUTTON, REAM_BUTTON, TEST_BUTTON};
const int numberOfButtons = 5;
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
    zero();
  }

  if(btn[BTN_REAM].fell()) // REAM
  {
    reamCycle();
  }

  if(btn[BTN_TEST].fell())
  {
    sendSerialMessage("\$r", getRPM());
  }

}