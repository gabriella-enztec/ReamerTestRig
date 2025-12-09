
#include "buttons.h"
#include "stepper_movement.h"
#include "comms.h"
#include "ream.h"
#include "state_machine.h"

#define SENDMESSAGE_PERIOD 25 //ms

long sendmessage_previous_time = 0;

// Initialisation function
void setup() 
{
  buttons_init();
  stepper_movement_init();
  serial_init();
  state_machine_init();
}


void loop() 
{
  checkSerialCommand(); // Checks Commands Sent from DEWESoft
  checkButtons(); // Checks Physical Buttons

}
