
#include "stepper_movement.h"
#include "state_machine.h"
#include "comms.h"
#include "settings.h"


#define CLEARANCE 10 // mm
#define STARTINGCLEARANCE -2 // mm


// Reaming Instructions
void reamCycle()
{
  // Move to 2mm above block
  setPositioning();
  move(STARTINGCLEARANCE, RAPIDSPEED);

  // Reaming cut
  setReaming();
  sendAllSerial();
  move(getDepthOfCut(), getFeedRate());

  //Extraction
  setRetracting();
  move(STARTINGCLEARANCE, RAPIDSPEED);
  sendAllSerial();
}

/*
void zeroCycle()
{

}
*/