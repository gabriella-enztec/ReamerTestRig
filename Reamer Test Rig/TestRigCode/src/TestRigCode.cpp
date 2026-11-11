
/*
 * main.cpp
 *
 * Purpose:
 *  - Central scheduler for system tasks using non-blocking timing (millis).
 *  - Periodically services communication, buttons, sequences, and motion control.
 *
 * Design notes:
 *  - All timing is cooperative (no delays).
 *  - Each subsystem is responsible for maintaining its own internal state.
 */

#include <Arduino.h>
#include "HeaderFiles/buttons.h"
#include "HeaderFiles/motion_controller.h"
#include "HeaderFiles/sequenceManager.h"
#include "HeaderFiles/stepper_movement.h"
#include "HeaderFiles/comms.h"
#include "HeaderFiles/settings.h"
#include "HeaderFiles/servo.h"

// Task execution periods (milliseconds)
#define SENDMESSAGE_PERIOD          50   // Periodic telemetry output to DEWESoft
#define CHECKSERIAL_PERIOD          50   // Incoming command processing
#define CHECKPHYSICALBUTTON_PERIOD  50   // Physical button debounce & handling
#define SEQUENCEMANAGER_PERIOD      50   // High-level sequence state machine
#define MOTIONCONTROLLER_PERIOD      5   // High-frequency motion control update


// Timestamp bookkeeping for cooperative task scheduling
long sendmessage_previous_time = 0;
long checkphysicalbutton_previous_time = 0;
long checkserialcommand_previous_time = 0;
long sequencemanager_previous_time = 0;
long motioncontroller_period = 0;

// Current system time (milliseconds since boot)
long currenttime = 0;


/*
 * setup()
 *
 * Performs one-time system initialisation:
 *  - Input devices (buttons)
 *  - Motion hardware (stepper drivers)
 *  - Serial communications
 *  - Persistent or runtime settings
 */
void setup() 
{
  buttons_init();
  stepper_movement_init();
  serial_init();
  initSettings();
  servo_init();
}


/*
 * loop()
 *
 * Main cooperative scheduler.
 * Each task is executed when its associated period has elapsed.
 * No blocking delays are used to maintain responsive motion control.
 */
void loop() 
{
  currenttime = millis();

  // Add a function which runs stepper motor

   // Periodic telemetry transmission
  if (currenttime - sendmessage_previous_time >= SENDMESSAGE_PERIOD)
  {
    sendAllSerial(); // Sends infomation to DEWESoft
    sendmessage_previous_time = currenttime;
  }

  // Process incoming serial commands
  if (currenttime - checkserialcommand_previous_time >= CHECKSERIAL_PERIOD)
  {
    checkSerialCommand(); // Checks Commands Sent from DEWESoft
    checkserialcommand_previous_time = currenttime;
  }

  // Read and debounce physical buttons
  if (currenttime - checkphysicalbutton_previous_time >= CHECKPHYSICALBUTTON_PERIOD)
  {
    checkButtons(); // Checks Physical Buttons
    checkphysicalbutton_previous_time = currenttime;
  }

  // Update high-level sequence state machine
  if (currenttime - sequencemanager_previous_time >= SEQUENCEMANAGER_PERIOD)
  {
    updateSequences(); // Checks for commands to respond to
    sequencemanager_previous_time = currenttime;
  }

  // High-rate motion control update
  if (currenttime - motioncontroller_period >= MOTIONCONTROLLER_PERIOD)
  {
    checkMotion(); // Updates Stepper Motor Function
    motioncontroller_period = currenttime;
  }

  // Stepper pulse generation (must run as fast as possible)
  runStepper();

}
