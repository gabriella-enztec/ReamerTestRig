
#include "buttons.h"
#include "stepper_movement.h"
#include "comms.h"
#include "ream.h"
#include "state_machine.h"
#include "settings.h"

#define SENDMESSAGE_PERIOD 200 // ms
#define CHECKSERIAL_PERIOD 100 // ms
#define CHECKPHYSICALBUTTON_PERIOD 1 // ms

long sendmessage_previous_time = 0;
long checkphysicalbutton_previous_time = 0;
long checkserialcommand_previous_time = 0;

long currenttime = 0;

// Initialisation function
void setup() 
{
  buttons_init();
  stepper_movement_init();
  serial_init();
  state_machine_init();
  initSettings();
}


void loop() 
{
  currenttime = millis();

  if (currenttime - sendmessage_previous_time >= SENDMESSAGE_PERIOD)
  {
    sendAllSerial(); // Sends infomation to DEWESoft
    sendmessage_previous_time = currenttime;
  }

  if (currenttime - checkserialcommand_previous_time >= CHECKSERIAL_PERIOD)
  {
    checkSerialCommand(); // Checks Commands Sent from DEWESoft
    checkserialcommand_previous_time = currenttime;
  }

  if (currenttime - checkphysicalbutton_previous_time >= CHECKPHYSICALBUTTON_PERIOD)
  {
    checkButtons(); // Checks Physical Buttons
    checkphysicalbutton_previous_time = currenttime;
  }

}
