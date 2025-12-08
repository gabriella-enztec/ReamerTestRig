
#include "stepper_movement.h"
#include <AccelStepper.h>

AccelStepper stepper(AccelStepper::DRIVER, StepPin, DirPin);


void move(int target_position, int speed)
{

}

void manualPositioning(bool upP, bool downP)
{
  if(!(upP && downP))
  {
    if(upP)
    {
      stepper.setSpeed(-RAPIDSPEED*STEPS_PER_MM);    // negative speed = opposite direction
      stepper.runSpeed();
    }
    else if (downP)
    {
      stepper.setSpeed(RAPIDSPEED*STEPS_PER_MM);    // negative speed = opposite direction
      stepper.runSpeed();
    }
  }
  else
  {
    stepper.setSpeed(0);
  }
}