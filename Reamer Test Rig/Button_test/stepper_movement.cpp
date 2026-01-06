
#include "stepper_movement.h"
#include "motion_controller.h"
#include "comms.h"
#include "settings.h"


// Stepper Motor Pins
#define DirPin 12       // Pin sets direction of rotation
#define StepPin 10      // Pin activates motion

float zeroingLoad = 10;

const float STEPS_PER_MM = 200.0f;

AccelStepper stepper(AccelStepper::DRIVER, StepPin, DirPin);

bool motionComplete = false;

bool targetupwards = false;
bool positioncalibrated = false;

float stepper_target_position = 0;
float stepper_speed = 0;

Direction previous_direction = STOP;

void stepper_movement_init()
{
  stepper.setMaxSpeed(1000);   
  stepper.setAcceleration(10000);
  stepper.setSpeed(0);
}


long getStepperPosition()
{
  return stepper.currentPosition();
}


float getStepperPositionmm()
{
    return (float)stepper.currentPosition()/STEPS_PER_MM;
}


void positionalMotion()
{
  stepper.moveTo(stepper_target_position * STEPS_PER_MM);

  if(stepper.currentPosition() < stepper_target_position && !positioncalibrated)
  {
    targetupwards = true;
    positioncalibrated = true;
  }
  else
  {
    targetupwards = false;
    positioncalibrated = true;
  }

  // Checks if target position is above current position
  if(targetupwards)
  {
    stepper.setSpeed(stepper_speed * STEPS_PER_MM);
    if (stepper.currentPosition() < stepper_target_position * STEPS_PER_MM) {
    }
    else
    {
      motionComplete = true;
      positioncalibrated = false;
    }
  }
  else   // Checks if target position is above below position
  {
    stepper.setSpeed(-stepper_speed *STEPS_PER_MM);
    if (stepper.currentPosition() > stepper_target_position * STEPS_PER_MM) {
    }
    else
    {
      motionComplete = true;
      positioncalibrated = false;
    }
  }
}


void directionalMotion(Direction direction)
{
  Serial.println("DirectionalMoition");
  stepper_speed = getStepperSpeed();

  if (direction == UP && previous_direction != UP)
  {
    Serial.println("Up moving");
    Serial.println(stepper_speed);
    stepper.setSpeed(stepper_speed*STEPS_PER_MM);    // negative speed = upwards direction
    previous_direction = UP;
  }
  else if (direction == DOWN && previous_direction != DOWN)
  {
    Serial.println("Down moving");
    Serial.println(stepper_speed);
    stepper.setSpeed(stepper_speed *STEPS_PER_MM);    // postive speed = downwards direction
    previous_direction = DOWN;
  }
  else if (direction == STOP && previous_direction != STOP)
  {
    Serial.println("Stop Moving");
    stepper.setSpeed(0);
    previous_direction = STOP;
  }
}


void runStepper()
{
  stepper.runSpeed();
}


void idle()
{
  stepper.setSpeed(0);
}


void zero()
{
  stepper.setCurrentPosition(0);
}


bool getMotionComplete()
{
  if (motionComplete)
  {
    motionComplete = false;
    return true;
  }
  else
  {
    return false;
  }
}