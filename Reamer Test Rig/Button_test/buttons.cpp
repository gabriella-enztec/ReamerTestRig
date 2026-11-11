
/*
 * buttons.cpp
 *
 * File Overview:
 *  - Handles all physical button input using the Bounce2 debounce library.
 *  - Translates button press and release events into high-level system actions.
 *  - Provides manual motion control and sequence triggering via hardware inputs.
 *
 * Design Notes:
 *  - Buttons are configured with internal pull-ups and active-low logic.
 *  - Debouncing is handled entirely by the Bounce2 library.
 *  - This module does not directly control hardware; it issues requests to
 *    higher-level controllers.
 */

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


/*
 * buttons_init()
 *
 * Configures button input pins and initializes debouncing.
 */
void buttons_init()
{
  // Set up for all the different buttons
  for (int i = 0; i < numberOfButtons; i++) 
  {
    pinMode(btnPins[i], INPUT_PULLUP);
    btn[i].attach(btnPins[i]);
    btn[i].interval(50);  // 50ms debounce
  }
}


/*
 * checkButtons()
 *
 * Checks the current state of all buttons and triggers actions accordingly.
 * This function should be called regularly in the main loop.
 */
void checkButtons()
{
  // Update all button states
  for (int i = 0; i < numberOfButtons; i++) {
    btn[i].update();  // Update debounce logic for this button
  }


  // --- Manual Down Movement ---
  if(btn[BTN_DOWN].fell())  // Button pressed (active-low)
  {
    requestManualMove(DOWN);  // Request manual downward motion
    //Serial.println("DOWN");
  }
  else if (btn[BTN_DOWN].rose())  // Button released
  {
    requestManualMove(STOP);  // Stop motion when button released
  }


  // --- Manual Up Movement ---
  if(btn[BTN_UP].fell()) // Button pressed
  {
    requestManualMove(UP);  // Request manual upward motion
    //Serial.println("UP");

  }
  else if (btn[BTN_UP].rose()) // Button released
  {
    requestManualMove(STOP);  // Stop motion when button released
  }


  // --- Zero Sequence Trigger --
  if(btn[BTN_ZERO].fell()) // Button pressed
  {
    setActiveSequence(SEQ_ZERO);  // Trigger the zeroing sequence
  }


  // --- Ream Sequence Trigger ---
  if(btn[BTN_REAM].fell()) // Button pressed
  {
    setActiveSequence(SEQ_REAM);  // Trigger the ream sequence
  }

}