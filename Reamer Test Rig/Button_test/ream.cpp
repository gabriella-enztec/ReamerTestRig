
/*
 * ream_sequence.cpp
 *
 * File Overview:
 *  - Implements the reaming operation as a multi-step state machine.
 *  - Coordinates positioning, reaming, and extraction movements using the
 *    motion controller interface.
 *  - Designed to be called repeatedly by the sequence manager in a
 *    non-blocking manner.
 *
 * Reaming Process:
 *  1. POSITION  – Move tool to starting clearance position
 *  2. REAM      – Feed tool to depth of cut
 *  3. EXTRACT   – Retract tool back to clearance
 *
 * Design Notes:
 *  - Each step issues a motion command once, then waits for completion.
 *  - State transitions occur only when positioningFinished() returns true.
 *  - The sequence self-resets on completion.
 */

#include "stepper_movement.h"
#include "motion_controller.h"
#include "sequenceManager.h"
#include "comms.h"
#include "settings.h"


// Clearance distance for safe positioning (mm)
#define CLEARANCE 10

// Initial clearance position before reaming (mm)
const float STARTINGCLEARANCE = -2.00;


/*
 * ReamSteps
 *
 * Defines the individual steps of the reaming sequence.
 */
enum ReamSteps
{
  POSITION,   // Move to starting clearance position
  REAM,       // Perform reaming operation
  EXTRACT,    // Retract tool after reaming
  NOPE        // Idle / inactive state
};

// Reaming sequence state tracking
ReamSteps current_step = POSITION;
ReamSteps previous_step = NOPE;
bool ream_finished = false;


/*
 * reamReset()
 *
 * Resets the reaming sequence to its initial state.
 * Clears all state flags and prepares for a new cycle.
 */
void reamReset()
{
  current_step = POSITION;
  previous_step = NOPE;
  ream_finished = false;
}


/*
 * reamSequence()
 *
 * Executes the reaming sequence state machine.
 * This function should be called periodically by the sequence manager.
 * Each step issues its motion command once and waits for completion
 * before advancing to the next step.
 */
void reamSequence()
{
  switch (current_step)
  {
    case POSITION:
      // Move tool to starting clearance position
      if (previous_step != POSITION)
      {
        positionalMovement(STARTINGCLEARANCE, getManualFeedRate());
        previous_step = POSITION;
      }
      else if (positioningFinished())
      {
        current_step = REAM;
        return;
      }
      break;
    
    case REAM:
      // Perform reaming operation to depth of cut
      if (previous_step != REAM)
      {
        positionalMovement(getDepthOfCut(), getFeedRate());
        previous_step = REAM;
      }
      else if (positioningFinished())
      {
        current_step = EXTRACT;
        return;
      }
      break;
    
    case EXTRACT:
      // Retract tool back to clearance position
      if(previous_step != EXTRACT)
      {
        positionalMovement(STARTINGCLEARANCE, getManualFeedRate());
        previous_step = EXTRACT;
      }
      else if (positioningFinished())
      {
        current_step = NOPE;
        ream_finished = true;

        // Reset sequence and return control to idle state
        reamReset();
        setActiveSequence(SEQ_NONE);
        return;
      }
      break;
  }
}


/*
 * reamFinished()
 *
 * Indicates whether the reaming sequence has completed.
 * Returns true once per completed sequence.
 */
bool reamFinished()
{
  if (ream_finished)
  {
    reamReset();
    return true;
  }
  else
  {
    return false;
  }
}


/*
 * isReaming()
 *
 * Indicates whether the system is currently performing the reaming step.
 *
 * Returns:
 *  -  1 : Reaming in progress
 *  - -1 : Not currently reaming
 */
int isReaming()
{
  if (current_step == REAM)
  {
    return 1;
  }
  else
  {
    return -1;
  }
}

