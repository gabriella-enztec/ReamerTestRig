#include "zero.h"
#include "settings.h"
#include "sequenceManager.h"
#include "stepper_movement.h"

enum ZeroSteps
{
  STARTINGLOAD,
  CHECKLOAD,
  RETURN,
  Z_IDLE
};

ZeroSteps step = STARTINGLOAD;
ZeroSteps Z_previous_step = Z_IDLE;

const float zeroingload = 10.0;
const float finishingDistance = -2.0;
const float zeroingSpeed = 0.2; // mm/s

static float starting_load = 0;

bool zero_finished = false;

void zeroSequenceUpdate()
{
  switch (step)
  {
    case (STARTINGLOAD):
      starting_load = getLoad();
      step = CHECKLOAD;
      Z_previous_step = STARTINGLOAD;
      break;

    case (CHECKLOAD):
      manualMovement(DOWN, zeroingSpeed);
      Z_previous_step = CHECKLOAD;
      if(getLoad() >= starting_load + zeroingload)
      {
        zero();
        step = RETURN;
      }
      break;
    
    case (RETURN):
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
      break;
  }
}


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
