
#include "HeaderFiles/stepper_movement.h"

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
  Serial.println("Going to starting positon");
  move(STARTINGCLEARANCE, RAPIDSPEED);

  // Reaming cut
  Serial.println("Reaming");
  move(REAMINGDEPTH, REAMINGFEEDRATE);

  //Extraction
  Serial.println("Extracting");
  move(STARTINGCLEARANCE, RAPIDSPEED);
}

// Drilling Instructions
void drillCycle()
{
  // Move to 2mm above block
  move(STARTINGCLEARANCE, RAPIDSPEED);

  // Reaming cut
  move(DRILLINGDEPTH, DRILLINGFEEDRATE);

  //Extraction
  move(STARTINGCLEARANCE, RAPIDSPEED);
}