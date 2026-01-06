
#ifndef STEPPER_MOVEMENT_H_
#define STEPPER_MOVEMENT_H_

#include <AccelStepper.h>
#include "motion_controller.h"

void move(float target_position, float speed);
void manualPositioning(bool upP, bool downP);
void manualPositioningWSpeed(bool upP, bool downP, float speed);
void stepper_movement_init();
void zero();

long getStepperPosition();
float getStepperPositionmm();
bool getMotionComplete();

void directionalMotion(Direction direction);
void positionalMotion();
void idle();
void zero();

#endif