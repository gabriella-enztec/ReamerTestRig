
#include "stepper_movement.h"
#include "motion_controller.h"
#include "sequenceManager.h"
#include "comms.h"
#include "settings.h"


#define CLEARANCE 10 // mm
const float STARTINGCLEARANCE = -2.00; // mm

enum ReamSteps
{
  POSITION,
  REAM,
  EXTRACT,
  NOPE
};

ReamSteps current_step = POSITION;
ReamSteps previous_step = NOPE;
bool ream_finished = false;


void reamReset()
{
  current_step = POSITION;
  previous_step = NOPE;
  ream_finished = false;
}


void reamSequence()
{
  switch (current_step)
  {
    case POSITION:
      if (previous_step != POSITION)
      {
        Serial.println("Set Positional Movement (Position)");
        positionalMovement(STARTINGCLEARANCE, getManualFeedRate());
        previous_step = POSITION;
      }

      else if (positioningFinished())
      {
        Serial.println("Finsih positioning Reaming");
        current_step = REAM;
        return;
      }
      break;
    
    case REAM:
      if (previous_step != REAM)
      {
        Serial.println("Set Positional Movement (REAM)");
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
      if(previous_step != EXTRACT)
      {
        Serial.println("Set Positional Movement (EXTRACT)");
        positionalMovement(STARTINGCLEARANCE, getManualFeedRate());
        previous_step = EXTRACT;
      }
      else if (positioningFinished())
      {
        current_step = NOPE;
        ream_finished = true;
        reamReset();
        setActiveSequence(SEQ_NONE);
        return;
      }
      break;
  }
}


bool reamFinished()
{
  if (ream_finished)
  {
    Serial.println("Reaming Finished");
    reamReset();
    return true;
  }
  else
  {
    return false;
  }
}


