
#include "stepper_movement.h"
#include "motion_controller.h"
#include "comms.h"
#include "settings.h"


#define CLEARANCE 10 // mm
#define STARTINGCLEARANCE -2 // mm

enum ReamSteps
{
  POSITION,
  REAM,
  EXTRACT
};

ReamSteps current_step = POSITION;
bool ream_finished = false;


void reamSequence()
{
  switch (current_step)
  {
    case POSITION:
      positionalMovement(STARTINGCLEARANCE, getManualFeedRate());
      if (positioningFinished())
      {
        current_step = REAM;
      }
      break;
    
    case REAM:
      positionalMovement(getDepthOfCut(), getFeedRate());
      if (positioningFinished())
      {
        current_step = EXTRACT;
      }
      break;
    
    case EXTRACT:
      positionalMovement(STARTINGCLEARANCE, getManualFeedRate());
      if (positioningFinished())
      {
        current_step = REAM;
        ream_finished = true;
      }
      break;
  }
}


bool reamFinished()
{
  if (ream_finished)
  {
    ream_finished = false;
    return true;
  }
  else
  {
    return false;
  }
}

