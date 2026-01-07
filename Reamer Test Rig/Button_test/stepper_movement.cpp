
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

float stepper_current_position = 0;

Direction previous_direction = STOP;

void stepper_movement_init()
{
  stepper.setMaxSpeed(1000);   
  stepper.setAcceleration(10000);
  stepper.setSpeed(0);
  motionComplete = false;
  idle();
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
  // Only set direction on first call or direction change
  if(!positioncalibrated)
  {
    stepper_target_position = getTargetPosition() * STEPS_PER_MM;
    stepper_speed = getStepperSpeed();
    Serial.println("Calibrate");
    Serial.println(stepper.currentPosition() / STEPS_PER_MM);
    Serial.println(stepper_target_position / STEPS_PER_MM);
    Serial.println(stepper_speed);
    targetupwards = (stepper.currentPosition() < stepper_target_position);
    if(targetupwards)
    {
      stepper.setSpeed(stepper_speed* STEPS_PER_MM);
    }
    else
    {
      stepper.setSpeed(-stepper_speed* STEPS_PER_MM);
    }
    //stepper.setSpeed((targetupwards ? stepper_speed : -stepper_speed) * STEPS_PER_MM);
    positioncalibrated = true;
    motionComplete = false;  // Only reset here
  }

  stepper_current_position = stepper.currentPosition();
  Serial.print("Position:");
  Serial.println(stepper_current_position);
  
  // Check if target reached
  if(targetupwards && stepper_current_position >= stepper_target_position)
  {
    motionComplete = true;
    positioncalibrated = false;
    //Serial.println("Target Reach (positionalMotion)");
  }
  else if(!targetupwards && stepper_current_position <= stepper_target_position)
  {
    motionComplete = true;
    positioncalibrated = false;
    //Serial.println("Target Reach (positionalMotion)");
  }
}


void directionalMotion(Direction direction)
{
  stepper_speed = getStepperSpeed();

  if (direction == UP && previous_direction != UP)
  {

    stepper.setSpeed(-stepper_speed*STEPS_PER_MM);    // negative speed = upwards direction
    previous_direction = UP;
  }
  else if (direction == DOWN && previous_direction != DOWN)
  {
    stepper.setSpeed(stepper_speed *STEPS_PER_MM);    // postive speed = downwards direction
    previous_direction = DOWN;
  }
  else if (direction == STOP && previous_direction != STOP)
  {
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
    Serial.println("Motion Complete");
    motionComplete = false;
    return true;
  }
  else
  {
    return false;
  }
}