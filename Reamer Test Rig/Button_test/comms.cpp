
#include "comms.h"
#include "ream.h"
#include "stepper_movement.h"

// Serial communication settings
#define INPUT_BUFFER_LEN 64
String input;

// Serial command timing
unsigned long lastCommandTime = 0;
const unsigned long debounceDelay = 50;


void serial_init()
{
    // Initialize serial communication
  Serial.begin(9600, SERIAL_8N1);
  input.reserve(INPUT_BUFFER_LEN);
}


// Checks for commands sent by DEWESoft
void checkSerialCommand()
{
  if (Serial.available()) {
    char input_char = (char)Serial.read();
    // Debounce serial commands
    if (millis() - lastCommandTime > debounceDelay) {
      switch (input_char) {
        case '0': // REAM COMMAND
          reamCycle();
          lastCommandTime = millis();
          break;
          
        case '1': // ZERO COMMAND
          zero();
          lastCommandTime = millis();
          break;
          
        case '2': // DRILL COMMAND
          drillCycle();
          lastCommandTime = millis();
          break;

        case '3': // UP COMMAND
          manualPositioning(true, false);
          break;

        case '4': // DOWN COMMAND
          manualPositioning(false, true);
          break;

        default:
          break;
      }
    }
  }
}
