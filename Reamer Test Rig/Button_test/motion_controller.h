#ifndef MOTION_CONTROLLER_H
#define MOTION_CONTROLLER_H

enum MotionState
{
  IDLE,
  DIRECTIONAL,
  POSITIONAL
};

enum Direction
{
  UP,
  DOWN,
  STOP
};

void checkMotion();

void manualMovement(Direction dir, float sp);
void positionalMovement(float tp, float sp);

float getStepperSpeed();
float getTargetPosition();

bool positioningFinished();

#endif