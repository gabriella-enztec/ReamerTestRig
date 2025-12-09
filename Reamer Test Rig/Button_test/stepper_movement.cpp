
#include "stepper_movement.h"

// Stepper Motor Pins
#define DirPin 12       // Pin sets direction of rotation
#define StepPin 10      // Pin activates motion
#define STEPS_PER_MM 200 // steps

AccelStepper stepper(AccelStepper::DRIVER, StepPin, DirPin);


void stepper_movement_init()
{
  stepper.setMaxSpeed(1000);    
  stepper.setAcceleration(10000); 
}


// Sets Zero
void zero()
{
  stepper.setCurrentPosition(0);
}


// Positional Movement 
void move(int target_position, float speed)
{
  stepper.moveTo(target_position * STEPS_PER_MM);

  // Checks if target position is above current position
  if(stepper.currentPosition() < target_position * STEPS_PER_MM)
  {
    stepper.setSpeed(speed * STEPS_PER_MM);
    while (stepper.currentPosition() < target_position * STEPS_PER_MM) {
      stepper.runSpeed();
    }
  }
  else   // Checks if target position is above below position
  {
    stepper.setSpeed(-speed *STEPS_PER_MM);
    while (stepper.currentPosition() > target_position * STEPS_PER_MM) {
      stepper.runSpeed();
    }
  }
}

c
// Directional Manual Movement
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