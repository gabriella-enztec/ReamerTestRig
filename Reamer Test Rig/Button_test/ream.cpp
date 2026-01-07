
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
        Serial.println("Positoing");
        positionalMovement(STARTINGCLEARANCE, getManualFeedRate());
        previous_step = POSITION;
      }

      else if (positioningFinished())
      {
        Serial.println("Positioing Finsihed");
        current_step = REAM;
        return;
      }
      break;
    
    case REAM:
      if (previous_step != REAM)
      {
        positionalMovement(getDepthOfCut(), getFeedRate());
        previous_step = REAM;
      }

      else if (positioningFinished())
      {
        Serial.println("REAMING Finsihed");
        current_step = EXTRACT;
        return;
      }
      break;
    
    case EXTRACT:
      if(previous_step != EXTRACT)
      {
        positionalMovement(STARTINGCLEARANCE, getManualFeedRate());
        previous_step = EXTRACT;
      }
      else if (positioningFinished())
      {
        Serial.println("EXTRACTING Finsihed");
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
    reamReset();
    return true;
  }
  else
  {
    return false;
  }
}


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

