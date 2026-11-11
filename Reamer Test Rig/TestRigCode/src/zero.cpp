
/*
 * zero.cpp
 *
 * File Overview:
 *  - Implements the zeroing sequence for the machine.
 *  - Controls downward manual motion until a load threshold is reached,
 *    then performs a zeroing action and returns to a finishing distance.
 *  - Provides a state-machine based sequence to ensure safe and repeatable zeroing.
 *
 * Key Variables:
 *  - step: Tracks the current zeroing step (STARTINGLOAD, CHECKLOAD, RETURN, Z_IDLE)
 *  - Z_previous_step: Tracks the previous step for proper initialization of movements
 *  - starting_load: Records the load at the start of zeroing
 *  - zero_finished: Flag indicating if zeroing sequence is complete
 *
 * Sequence Steps:
 * 1. STARTINGLOAD: Capture initial load measurement.
 * 2. CHECKLOAD: Move downward until load threshold is met, then trigger zero().
 * 3. RETURN: Move to finishing distance using positional movement.
 * 4. Z_IDLE: Idle step, no movement.
 *
 * Functions:
 *  - zeroSequenceUpdate(): Advances the zeroing sequence; called periodically.
 *  - zeroFinished(): Returns true if the zeroing sequence has finished since last check.
 */
 
#include "HeaderFiles/zero.h"
#include "HeaderFiles/settings.h"
#include "HeaderFiles/sequenceManager.h"
#include "HeaderFiles/stepper_movement.h"

// Zeroing Sequence Steps
enum ZeroSteps
{
  STARTINGLOAD,   // Record starting load for zeroing
  CHECKLOAD,      // Move down until specified load is reached
  RETURN,         // Move back to finishing distance after zeroing
  Z_IDLE          // Idle step
};

// Current and previous step trackers
ZeroSteps step = STARTINGLOAD;
ZeroSteps Z_previous_step = Z_IDLE;

// Zeroing parameters
const float zeroingload = 10.0;           // Load threshold to trigger zeroing
const float finishingDistance = -2.0;     // Distance to move back after zeroing
const float zeroingSpeed = 0.2;           // Manual movement speed during zeroing (mm/s)

static float starting_load = 0;            // Stores load value at start of zeroing
bool zero_finished = false;                // Flag indicating if zeroing sequence is complete


/*
 * zeroSequenceUpdate()
 *
 * Updates the zeroing sequence state machine.
 * Should be called periodically to progress through zeroing steps.
 * 
 * Sequence:
 * 1. STARTINGLOAD: Records initial load.
 * 2. CHECKLOAD: Moves down until zeroing load threshold is reached, then triggers zero().
 * 3. RETURN: Moves to finishing distance and marks sequence as finished.
 */
void zeroSequenceUpdate()
{
  switch (step)
  {
    case (STARTINGLOAD):
      // Gets the starting load of the rig (zeroing)
      starting_load = getLoad();       
      step = CHECKLOAD;                
      Z_previous_step = STARTINGLOAD;
      break;

    case (CHECKLOAD):
      // Moves rig downwards until zeroing load has been applied
      manualMovement(DOWN, zeroingSpeed);
      Z_previous_step = CHECKLOAD;

      if(getLoad() >= starting_load + zeroingload)
      {
        zero();
        step = RETURN;
      }
      break;
    
    case (RETURN):
      // Moves reamer to starting posistion
      if (Z_previous_step != RETURN)
      {
        positionalMovement(finishingDistance, getManualFeedRate());
        Z_previous_step = RETURN;
      }
      else if (positioningFinished())
      {
        step = STARTINGLOAD;
        zero_finished = true;
        setActiveSequence(SEQ_NONE);
      }
      break;

    default:
      // Do nothing for undefined steps
      break;
  }
}


/*
 * zeroFinished()
 *
 * Returns true if the zeroing sequence has completed since last check.
 * Resets the zero_finished flag and clears active sequence.
 */
bool zeroFinished()
{
  if (zero_finished)
  {
    zero_finished = false;
    setActiveSequence(SEQ_NONE);
    return true;
  }
  else
  {
    return false;
  }
}
