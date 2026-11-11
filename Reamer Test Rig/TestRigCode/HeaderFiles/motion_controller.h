#ifndef MOTION_CONTROLLER_H
#define MOTION_CONTROLLER_H

/*
 * motion_controller.h
 *
 * File Overview:
 *  - Public interface for the motion controller module.
 *  - Defines motion states, direction commands, and control functions
 *    used by higher-level modules (e.g. sequence manager, communications).
 *  - Encapsulates motion logic for both manual and positional stepper control.
 *
 * Usage Notes:
 *  - checkMotion() must be called periodically by the main scheduler.
 *  - Motion parameters such as speed and target position are internally
 *    managed and should not be modified directly.
 */

/*
 * MotionState
 *
 * Represents the current operating mode of the motion controller.
 */
enum MotionState
{
  IDLE,         // No active motion
  DIRECTIONAL,  // Continuous motion in a specified direction
  POSITIONAL    // Closed-loop motion to a target position
};

/*
 * Direction
 *
 * Direction commands for manual motion control.
 */
enum Direction
{
  UP,    // Move in the positive direction
  DOWN,  // Move in the negative direction
  STOP   // Stop motion and return to IDLE
};

/*
 * checkMotion()
 *
 * Executes the motion controller state machine.
 * Should be called at a fixed, high-frequency interval.
 */
void checkMotion();


/*
 * manualMovement()
 *
 * Initiates continuous motion in a specified direction.
 *
 * Parameters:
 *  - dir : Direction of movement
 *  - sp  : Speed command (mm/s)
 */
void manualMovement(Direction dir, float sp);

/*
 * positionalMovement()
 *
 * Initiates motion toward a target position.
 *
 * Parameters:
 *  - tp : Target position
 *  - sp : Speed command (mm/s)
 */
void positionalMovement(float tp, float sp);

/*
 * getStepperSpeed()
 *
 * Returns the current commanded stepper speed.
 */
float getStepperSpeed();

/*
 * positioningFinished()
 *
 * Indicates whether a positional move has completed.
 * Returns true once per completed move.
 */
float getTargetPosition();

bool positioningFinished();

#endif  // MOTION_CONTROLLER_H