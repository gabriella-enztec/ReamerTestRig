#include <Bounce2.h>
#include "buttons.h"
#include "ream.h"
#include "stepper_movement.h"
#include "motion_controller.h"
#include "comms.h"
#include "settings.h"
#include "sequenceManager.h"

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
    manualMovement(DOWN, getManualFeedRate());
  }
  else if (btn[BTN_DOWN].rose())
  {
    manualMovement(STOP, getManualFeedRate());
  }

  if(btn[BTN_UP].fell()) // UP
  {
    manualMovement(UP, getManualFeedRate());
  }
  else if (btn[BTN_UP].rose())
  {
    manualMovement(STOP, getManualFeedRate());
  }

  if(btn[BTN_ZERO].fell()) // ZERO
  {
    Serial.println("ZERO BUTTON");
    setActiveSequence(SEQ_ZERO);
  }

  if(btn[BTN_REAM].fell()) // REAM
  {
    Serial.println("REAM BUTTON");
    setActiveSequence(SEQ_REAM);
  }

}