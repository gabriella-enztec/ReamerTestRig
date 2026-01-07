#include <Arduino.h>
#include "motion_controller.h"
#include "stepper_movement.h"
#include "settings.h"
#include "comms.h"

static Direction direction = UP;
static MotionState motion_mode = IDLE;
static float movement_speed = 0;
static float target_position = 0;

bool positoning_finished = false;


void manualMovement(Direction dir, float sp)   // ✅ No trailing comma
{
  direction = dir;
  movement_speed = sp;
  motion_mode = DIRECTIONAL;
  positoning_finished = false;
}


void positionalMovement(float tp, float sp)
{
  target_position = tp;
  movement_speed = sp;
  positoning_finished = false;
  motion_mode = POSITIONAL;
}


void checkMotion()
{
  switch (motion_mode)
  {
    case (IDLE):
      idle();
      break;

    case (DIRECTIONAL):
      directionalMotion(direction);
      if(direction == STOP)
      {
        motion_mode = IDLE;
      }
      break;

    case (POSITIONAL):
      positionalMotion();
      if (getMotionComplete())
      {
        positoning_finished = true;
        motion_mode = IDLE;
      }
      break;
    
    default:
      break;
  }

  //sendPosistion();
}


bool positioningFinished()
{
  if (positoning_finished)
  {
    Serial.println("PositingFinsihedFunc");
    positoning_finished = false;
    return true;
  }
  else
  {
    return false;
  }
}


float getStepperSpeed()
{
  return movement_speed;
}


float getTargetPosition()
{
  return target_position;
}
