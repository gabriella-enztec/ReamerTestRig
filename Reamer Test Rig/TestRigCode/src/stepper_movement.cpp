
/*
 * stepper_movement.cpp
 *
 * File Overview:
 *  - Implements low-level stepper motor control using the AccelStepper library.
 *  - Converts high-level motion commands (directional / positional) into
 *    stepper speed and position updates.
 *  - Handles stepper calibration, target tracking, and motion completion flags.
 *
 * Design Notes:
 *  - This module does NOT decide *what* motion to perform; it only executes
 *    commands issued by the motion_controller module.
 *  - All motion is non-blocking and must be serviced continuously via runStepper().
 *  - Positioning is performed in open-loop steps, assuming a calibrated system.
 */

#include "HeaderFiles/stepper_movement.h"
#include "HeaderFiles/motion_controller.h"
#include "HeaderFiles/comms.h"
#include "HeaderFiles/settings.h"


// Stepper motor control pins
#define DirPin  12    // Direction control pin
#define StepPin 10    // Step pulse pin PWM!!!!!

// Load flag for zeroing
float zeroingLoad = 10;

// Mechanical conversion constant
const float STEPS_PER_MM = 200.0f;

// AccelStepper instance (driver mode)
AccelStepper stepper(AccelStepper::DRIVER, StepPin, DirPin);

// Motion state flags
bool motionComplete = false;       // Indicates completion of positional movement
bool targetupwards = false;        // Direction toward target position
bool positioncalibrated = false;   // True once target parameters are latched

// Motion parameters
float stepper_target_position = 0;   // Target position (steps)
float stepper_speed = 0;             // Speed command (mm/s)
float stepper_current_position = 0;  // Current position (steps)

// Used to prevent redundant speed updates
Direction previous_direction = STOP;


/*
 * stepper_movement_init()
 *
 * Initializes the stepper motor parameters and places the system
 * into a safe idle state.
 */
void stepper_movement_init()
{
  stepper.setMaxSpeed(1000);   
  stepper.setAcceleration(10000);
  stepper.setSpeed(0);

  motionComplete = false;
  idle();
}


/*
 * getStepperPosition()
 *
 * Returns the current stepper position in steps.
 */
long getStepperPosition()
{
  return stepper.currentPosition();
}


/*
 * getStepperPositionmm()
 *
 * Returns the current stepper position converted to millimeters.
 */
float getStepperPositionmm()
{
    return (float)stepper.currentPosition()/STEPS_PER_MM;
}


/*
 * positionalMotion()
 *
 * Executes a positional move toward a target position.
 * Target parameters are latched once per movement to prevent reconfiguration
 * during motion.
 */
void positionalMotion()
{
  // Only set direction on first call or direction change
  if(!positioncalibrated)
  {
    stepper_target_position = getTargetPosition() * STEPS_PER_MM;
    stepper_speed = getStepperSpeed();

    // Determine direction of travel
    targetupwards = (stepper.currentPosition() < stepper_target_position);

     // Apply signed speed based on direction
    if(targetupwards)
    {
      stepper.setSpeed(stepper_speed* STEPS_PER_MM);
    }
    else
    {
      stepper.setSpeed(-stepper_speed* STEPS_PER_MM);
    }

    positioncalibrated = true;
    motionComplete = false;
  }

  stepper_current_position = stepper.currentPosition();
  
  // Check if target reached
  if(targetupwards && stepper_current_position >= stepper_target_position)
  {
    motionComplete = true;
    positioncalibrated = false;
  }
  else if(!targetupwards && stepper_current_position <= stepper_target_position)
  {
    motionComplete = true;
    positioncalibrated = false;
  }
}


/*
 * directionalMotion()
 *
 * Executes continuous motion in a specified direction.
 * Speed updates are only applied when the direction changes.
 *
 * Parameters:
 *  - direction : Desired direction of motion
 */
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


/*
 * runStepper()
 *
 * Advances the stepper motor based on the currently set speed.
 * Must be called as frequently as possible.
 */
void runStepper()
{
  stepper.runSpeed();
}


/*
 * idle()
 *
 * Stops all stepper motion without resetting position.
 */
void idle()
{
  stepper.setSpeed(0);
}


/*
 * zero()
 *
 * Sets the current stepper position as zero.
 * Used after homing or calibration.
 */
void zero()
{
  stepper.setCurrentPosition(0);
}

/*
 * getMotionComplete()
 *
 * Indicates whether a positional movement has completed.
 * Returns true once per completed movement and clears the flag.
 */
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