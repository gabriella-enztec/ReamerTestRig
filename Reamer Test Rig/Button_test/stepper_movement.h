
#ifndef STEPPER_MOVEMENT_H_
#define STEPPER_MOVEMENT_H_

#include <AccelStepper.h>

void move(float target_position, float speed);
void manualPositioning(bool upP, bool downP);
void manualPositioningWSpeed(bool upP, bool downP, float speed);
void stepper_movement_init();
void zero();
long getStepperPosition();
float getStepperPositionmm();

// Movement Speed Settings
#define MANUALSPEED 5 // mm/s
#define RAPIDSPEED 2 // mm/s
#define POSITIONSPEED 2 // mm/s

#endif