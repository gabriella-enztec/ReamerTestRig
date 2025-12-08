
#include "buttons.h"
#include "stepper_movement.h"
#include "comms.h"

// Initialisation function
void setup() 
{
  buttons_init();
  stepper_movement_init();
  serial_init();
}


void loop() 
{
  checkSerialCommand(); // Checks Commands Sent from DEWESoft
  checkButtons(); // Checks Physical Buttons
}
