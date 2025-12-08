// Simplified code just to check that both buttons are working as expected
// If the buttons dont work when pressed try shorting thier connections to see movement (touching the in wire to the out wire of the button)
// If that works the connections to the button are dodgy, if that doesnt work then refer to the wiring diagram to trouble shoot

#include <AccelStepper.h>     // Library controls stepper motion
#include <Bounce2.h>

#include "comms.h"
#include "stepper_movement.h"

// Defining pin positions
#define DirPin 12       // Pin sets direction of rotation
#define StepPin 10      // Pin activates motion
#define DownButton 2    // Button moves gantry downwards
#define UpButton 4      // Button moves gantry upwards
#define ZeroButton 8
#define ReamButton 6


const int btnPins[] = {DownButton, UpButton, ZeroButton, ReamButton};
const int numberOfButtons = 4;

Bounce btn[numberOfButtons];

AccelStepper stepper(AccelStepper::DRIVER, StepPin, DirPin); // stepPin, dirPin

// Global variables and constants
int ReamingDepth = 4; // Reaming depth in [mm]
float Feedrate = 0.5;     // Gantry feedrate in [mm/s]

int drillingdepth = 70;
float drillFeedrate = 2;

int RapidSpeed = 5; // Speed for rapid positioning movement [mm/s]
int PositionSpeed = 2;  // Speed for accurate positioning movement [mm/s]
int ManualSpeed = RapidSpeed; // Sets manual speed to rapid mode initially

bool upPressed = false;
bool downPressed = false;

unsigned long lastPress = 0;
const unsigned long debounceDelay = 50;

// Defining mechanical and timing constants
const float Lead = 8.0; // Lead of the screw (distance per revolution)
const int StepsPerRev = 1600; // Steps needed for one full turn
const int StepsPer_mm = StepsPerRev / Lead;

long reamSpeedSteps = StepsPer_mm * Feedrate;
long rapidSpeedSteps = StepsPer_mm * RapidSpeed;
long returnSpeedSteps = StepsPer_mm * PositionSpeed;

int startingGap = -2; // 2mm

bool busy = false;

long lastcommandtime = 0;
long delaytime = 50;


void setup() {

  for (int i = 0; i < numberOfButtons; i++)
  {
    pinMode(btnPins[i], INPUT_PULLUP);
    btn[i].attach(btnPins[i]);
    btn[i].interval(50);
  }

  stepper.setMaxSpeed(1000);     // steps/sec 
  stepper.setAcceleration(10000);  // steps/sec^2 : Set high to make acceleration instant during position control.

  //init serial terminal and reserve chars in input buffer.
  Serial.begin(9600, SERIAL_8N1);
  input.reserve(INPUT_BUFFER_LEN);
}


void loop() {

  if (Serial.available()) {
    char input_char = (char)Serial.read();
    if (millis() - lastcommandtime > delaytime)
    {
      if (input_char == '0') 
      {
        reamCycle();       // execute command
        busy = true;
      }
      else if (input_char == '1') {
        stepper.setCurrentPosition(0);
        busy = true;
      }
      if(busy)
      {
        lastcommandtime = millis();
        busy = false;
      }
    }
  } 


  for (int i = 0; i < numberOfButtons; i++) {
    btn[i].update();
  }

  if(btn[0].fell()) // Down
  {
    downPressed = true;
  }
  else if(btn[0].rose())
  {
    downPressed = false;
  }

  if(btn[1].fell()) // UP
  {
    upPressed = true;
  }
  else if(btn[1].rose())
  {
    upPressed = false;
  }

  if(btn[2].fell()) // ZERO
  {
    stepper.setCurrentPosition(0);
  }

  if(btn[3].fell()) // REAM
  {
    reamCycle();
  }

  manualPositioning(upPressed, downPressed);
}




void toReamStartingPosition()
{
  // --- 1. Move to +2 mm above surface (rapid) ---
  stepper.moveTo(startingGap * StepsPer_mm);
  if(stepper.currentPosition() < startingGap)
  {
    stepper.setSpeed(rapidSpeedSteps);
    while (stepper.currentPosition() < startingGap * StepsPer_mm) {
      stepper.runSpeed();
    }
  }
  else
  {
    stepper.setSpeed(-rapidSpeedSteps);
    while (stepper.currentPosition() > startingGap * StepsPer_mm) {
      stepper.runSpeed();
    }
  }
}


void reamCycle() {

  toReamStartingPosition();

  delay(300);

  // --- 2. Ream to -4 mm below surface (feed rate) ---
  stepper.moveTo(ReamingDepth * StepsPer_mm);
  stepper.setSpeed(reamSpeedSteps);  // negative because going downward
  while (stepper.currentPosition() < ReamingDepth * StepsPer_mm) {
    stepper.runSpeed();
  }

  delay(300);

  // --- 3. Retract to +10 mm above surface ---
  stepper.moveTo(-10 * StepsPer_mm);
  stepper.setSpeed(-rapidSpeedSteps);
  while (stepper.currentPosition() > -10 * StepsPer_mm) {
    stepper.runSpeed();
  }
}


void drillCycle() {
  // Stepper attributes
  long StepperSpeed = StepsPer_mm * drillFeedrate;  // Steps per second
  
  // Move to 1mm above block surface (rapid positioning - can use runToPosition)
  stepper.setSpeed(PositionSpeed * StepsPer_mm);
  stepper.moveTo(0);
  stepper.runToPosition();
  
  delay(1000);
  
  stepper.moveTo(drillingdepth * StepsPer_mm);
  stepper.setSpeed(StepperSpeed);  
  while (stepper.currentPosition() <= drillingdepth * StepsPer_mm) {
    stepper.runSpeed();
  }

  stepper.moveTo(0);
  stepper.setSpeed(-RapidSpeed * StepsPer_mm);
  while (stepper.currentPosition() >= 0) {
    stepper.runSpeed();
  }
}
