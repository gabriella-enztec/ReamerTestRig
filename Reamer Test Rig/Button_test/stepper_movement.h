
#ifndef STEPPER_MOVEMENT_H_
#define STEPPER_MOVEMENT_H_

#include <AccelStepper.h>

void move(int target_position, float speed);
void manualPositioning(bool upP, bool downP);
void stepper_movement_init();
void zero();

// Movement Speed Settings
#define RAPIDSPEED 5 // mm/s
#define POSITIONSPEED 2 // mm/s

#endif