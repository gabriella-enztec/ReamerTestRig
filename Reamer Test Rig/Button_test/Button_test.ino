
#include "buttons.h"
#include "stepper_movement.h"
#include "ream.h"
#include "comms.h"


void setup() 
{
  buttons_init();
  stepper_movement_init();
  serial_init();
}

void loop() 
{
  checkSerialCommand();
  checkButtons();
}
