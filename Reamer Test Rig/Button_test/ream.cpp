
#include "stepper_movement.h"
#include "state_machine.h"
#include "comms.h"

// Reaming Cutting Settings
#define REAMINGDEPTH 4 // mm
#define REAMINGFEEDRATE 0.5 // mm/s
#define CLEARANCE 10 // mm

// Drilling Cutting Settings
#define DRILLINGDEPTH 70 // mm
#define DRILLINGFEEDRATE 2 // mm/s

#define STARTINGCLEARANCE -2 // mm


// Reaming Instructions
void reamCycle()
{
  // Move to 2mm above block
  setPositioning();
  //move(STARTINGCLEARANCE, RAPIDSPEED);

  // Reaming cut
  setReaming();
  sendSerialMeasure();
  move(REAMINGDEPTH, REAMINGFEEDRATE);


  //Extraction
  setRetracting();
  move(STARTINGCLEARANCE, RAPIDSPEED);
  sendSerialMeasure();
}

// Drilling Instructions
void drillCycle()
{
  // Move to 2mm above block'
  setPositioning();
  move(STARTINGCLEARANCE, RAPIDSPEED);

  // Reaming cut
  setReaming();
  move(DRILLINGDEPTH, DRILLINGFEEDRATE);

  //Extraction
  setRetracting();
  move(STARTINGCLEARANCE, RAPIDSPEED);
}