
#include "stepper_movement.h"
#include "comms.h"
#include "settings.h"

// Stepper Motor Pins
#define DirPin 12       // Pin sets direction of rotation
#define StepPin 10      // Pin activates motion

float zeroingLoad = 10;

const float STEPS_PER_MM = 200.0f;

AccelStepper stepper(AccelStepper::DRIVER, StepPin, DirPin);


void stepper_movement_init()
{
  stepper.setMaxSpeed(1000);    
  stepper.setAcceleration(10000); 
}


// Sets Zero
void zero()
{
  float starting_load = getLoad();
  float load = 0;
  while (1)
  {
    checkSerialCommand();
    load = getLoad();
    if(load >= starting_load + zeroingLoad)
    {
      stepper.setCurrentPosition(0);
      manualPositioningWSpeed(false, false, 0);
      move(-2, 5);
      break;
    }
    else
    {
      manualPositioningWSpeed(false, true, 0.2);
    }
  }




}


// Positional Movement 
void move(float target_position, float speed)
{
  stepper.moveTo(target_position * STEPS_PER_MM);

  // Checks if target position is above current position
  if(stepper.currentPosition() < target_position * STEPS_PER_MM)
  {
    stepper.setSpeed(speed * STEPS_PER_MM);
    while (stepper.currentPosition() < target_position * STEPS_PER_MM) {
      stepper.runSpeed();
      sendPosistion();
    }
  }
  else   // Checks if target position is above below position
  {
    stepper.setSpeed(-speed *STEPS_PER_MM);
    while (stepper.currentPosition() > target_position * STEPS_PER_MM) {
      stepper.runSpeed();
      sendPosistion();
    }
  }
}


// Directional Manual Movement
void manualPositioning(bool upP, bool downP)
{
  if(!(upP && downP))
  {
    if(upP)
    {
      stepper.setSpeed(-RAPIDSPEED*STEPS_PER_MM);    // negative speed = opposite direction
      stepper.runSpeed();
      sendPosistion();
    }
    else if (downP)
    {
      stepper.setSpeed(RAPIDSPEED*STEPS_PER_MM);    // negative speed = opposite direction
      stepper.runSpeed();
      sendPosistion();
    }
  }
  else
  {
    stepper.setSpeed(0);
  }
}


void manualPositioningWSpeed(bool upP, bool downP, float speed)
{
  if(!(upP && downP))
  {
    if(upP)
    {
      stepper.setSpeed(-speed*STEPS_PER_MM);    // negative speed = opposite direction
      stepper.runSpeed();
      sendPosistion();
    }
    else if (downP)
    {
      stepper.setSpeed(speed*STEPS_PER_MM);    // negative speed = opposite direction
      stepper.runSpeed();
      sendPosistion();
    }
  }
  else
  {
    stepper.setSpeed(0);
  }
}


long getStepperPosition()
{
  return stepper.currentPosition();
}

float getStepperPositionmm()
{
    return (float)stepper.currentPosition()/STEPS_PER_MM;
}