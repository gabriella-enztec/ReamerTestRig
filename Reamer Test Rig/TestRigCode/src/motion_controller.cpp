
/*
 * motion_controller.cpp
 *
 * File Overview:
 *  - Implements the high-level motion control logic for the stepper system.
 *  - Manages motion modes including idle, manual directional movement, and
 *    closed-loop positional movement.
 *  - Acts as a state machine that translates commands from the sequence manager
 *    or communications interface into low-level stepper actions.
 *
 * Design Notes:
 *  - This module does not generate step pulses directly; that responsibility
 *    is handled by the stepper_movement module.
 *  - Motion updates are intended to be called periodically by the main scheduler
 *    (non-blocking, millis-based timing).
 *  - Internal state is kept static to prevent external modification.
 */


#include <Arduino.h>
#include "HeaderFiles/motion_controller.h"
#include "HeaderFiles/stepper_movement.h"
#include "HeaderFiles/sequenceManager.h"
#include "HeaderFiles/settings.h"
#include "HeaderFiles/comms.h"


/*
 * Internal motion control state
 * These variables define the current motion mode and parameters.
 * They are kept static to limit scope to this module.
 */
static Direction direction = UP;        // Current commanded direction
static MotionState motion_mode = IDLE;  // Current motion mode/state
static float movement_speed = 0;        // Speed command for the stepper
static float target_position = 0;       // Target position for positional moves

// Flag indicating completion of a positional movement
bool positoning_finished = false;


/*
 * manualMovement()
 *
 * Initiates a continuous directional movement.
 * Used for manual control where motion continues until a STOP command is issued.
 *
 * Parameters:
 *  - dir : Direction of motion (UP, DOWN, STOP)
 *  - sp  : Movement speed (mm/s)
 */
void manualMovement(Direction dir, float sp)   // ✅ No trailing comma
{
  direction = dir;
  movement_speed = sp;
  motion_mode = DIRECTIONAL;
  positoning_finished = false;
}


/*
 * positionalMovement()
 *
 * Initiates a closed-loop positional move to a target position.
 * Motion continues until the target position is reached.
 *
 * Parameters:
 *  - tp : Target position
 *  - sp : Movement speed (mm/s)
 */
void positionalMovement(float tp, float sp)
{
  target_position = tp;
  movement_speed = sp;
  positoning_finished = false;
  motion_mode = POSITIONAL;
}


/*
 * checkMotion()
 *
 * Main motion control state handler.
 * This function should be called periodically by the scheduler.
 * It dispatches motion behavior based on the current motion mode.
 */
void checkMotion()
{
  switch (motion_mode)
  {
    case (IDLE):
      // No active motion; ensure motors are in a safe idle state
      idle();
      break;

    case (DIRECTIONAL):
      // Continuous motion in a specified direction
      directionalMotion(direction);
      
      // Transition back to IDLE when STOP is commanded
      if(direction == STOP)
      {
        motion_mode = IDLE;
      }
      break;

    case (POSITIONAL):
      // Execute positional move toward target_position
      positionalMotion();

      // Check if target position has been reached
      if (getMotionComplete())
      {
        positoning_finished = true;
        motion_mode = IDLE;
      }
      break;
    
    default:
      // Safety fallback: do nothing
      break;
  }
}


/*
 * positioningFinished()
 *
 * Checks whether a positional movement has completed.
 * This function returns true once per completed movement
 * and automatically clears the completion flag.
 *
 * Returns:
 *  - true  : Positioning just completed
 *  - false : Positioning still in progress or not started
 */
bool positioningFinished()
{
  if (positoning_finished)
  {
    positoning_finished = false;
    return true;
  }
  else
  {
    return false;
  }
}


/*
 * getStepperSpeed()
 *
 * Returns the currently commanded stepper speed.
 */
float getStepperSpeed()
{
  return movement_speed;
}


/*
 * getTargetPosition()
 *
 * Returns the current target position for positional movement.
 */
float getTargetPosition()
{
  return target_position;
}
