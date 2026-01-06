#include "zero.h"
#include "settings.h"
#include "stepper_movement.h"

enum ZeroSteps
{
  STARTINGLOAD,
  CHECKLOAD,
  RETURN
};

ZeroSteps step = STARTINGLOAD;
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
      Serial.println("STARTINGLOAD ZERO");
      starting_load = getLoad();
      step = CHECKLOAD;
      break;

    case (CHECKLOAD):
      Serial.println("CHECKLOAD ZERO");
      manualMovement(DOWN, zeroingSpeed);
      if(getLoad() >= starting_load + zeroingload)
      {
        zero();
        step = RETURN;
      }
      break;
    
    case (RETURN):
      Serial.println("RETURN ZERO");
      positionalMovement(finishingDistance, getManualFeedRate());
      if (positioningFinished())
      {
        Serial.println("FINISHED ZERO");
        step = STARTINGLOAD;
        zero_finished = true;
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
    return true;
  }
  else
  {
    return false;
  }
}
