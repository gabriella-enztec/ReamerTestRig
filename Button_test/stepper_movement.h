
#ifndef STEPPER_MOVEMENT_H_
#define STEPPER_MOVEMENT_H_

#include <AccelStepper.h>

void move(int target_position, int speed);
void manualPositioning(bool upP, bool downP);

// Reaming Cutting Settings
#define REAMINGDEPTH 4 // mm
#define REAMINGFEEDRATE 0.5 // mm/s
#define CLEARANCE 10 // mm

// Drilling Cutting Settings
#define DRILLINGDEPTH 70 // mm
#define DRILLINGFEEDRATE 2 // mm/s

// Stepper Motor Pins
#define DirPin 12       // Pin sets direction of rotation
#define StepPin 10      // Pin activates motion

// Movement Speed Settings
#define RAPIDSPEED 5 // mm/s
#define POSITIONSPEED 2 // mm/s

#define STEPS_PER_MM 200 // steps

#endif