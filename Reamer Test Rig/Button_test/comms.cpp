
#include "comms.h"
#include "buttons.h"
#include "ream.h"
#include "stepper_movement.h"
#include "state_machine.h" 
#include "settings.h"

// Serial communication settings
#define INPUT_BUFFER_LEN 64
String input;
int serialMeasure = 0;

// Serial command timing
unsigned long lastCommandTime = 0;
const unsigned long debounceDelay = 50;

// Prefix Tx (Transmitting)
const String currentPosistionPrefix = "\$p";
const String startStoringPrefix    = "\$s";
const String rpmPrefix             = "\$r";
const String feedRatePrefix        = "\$f";
const String depthOfCutPrefix      = "\$d";

// Prefix Rx (Receiving)
const char startReamingPrefix[]           = "\$re";
const char zeroPrefix[]                   = "\$z";
const char manualUpPrefix[]               = "\$up";
const char manualDownPrefix[]             = "\$mown";
const char deafultReamingSettingPrefix[]  = "\$dreaming";
const char deafultDrillingSettingPrefix[] = "\$drilling";
const char increaseRPMPrefix[]            = "\$ri";
const char decreaseRPMPrefix[]            = "\$rd";
const char increaseFeedRatePrefix[]       = "\$fri";
const char decreaseFeedRatePrefix[]       = "\$frd";
const char increaseDepthOfCutPrefix[]     = "\$dci";
const char decreaseDepthOfCutPrefix[]     = "\$dcd";

// Postfix
const String postfix = "\\n";



void serial_init()
{
    // Initialize serial communication
  Serial.begin(9600, SERIAL_8N1);
  input.reserve(INPUT_BUFFER_LEN);
}

// Buffer for incoming serial command
char serialBuffer[20];
uint8_t bufferIndex = 0;


void checkSerialCommand()
{
    while (Serial.available())
    {
        char c = Serial.read();

        // End of command (postfix = "\n")
        if (c == '\n')
        {
            serialBuffer[bufferIndex] = '\0';  // Null-terminate

            // Debounce
            if (millis() - lastCommandTime > debounceDelay)
            {
                // Compare full commands
                if (strcmp(serialBuffer, startReamingPrefix) == 0) {      // "$re"
                    reamCycle();
                }
                else if (strcmp(serialBuffer, zeroPrefix) == 0) {         // "$z"
                    zero();
                }
                else if (strcmp(serialBuffer, deafultDrillingSettingPrefix) == 0) { // "$dd"
                    drillCycle();
                }
                else if (strcmp(serialBuffer, increaseRPMPrefix) == 0) {  // "$ri"
                    increaseRPM();
                }
                else if (strcmp(serialBuffer, decreaseRPMPrefix) == 0) {  // "$rd"
                    decreaseRPM();
                }
                else if (strcmp(serialBuffer, increaseFeedRatePrefix) == 0) { // "$fri"
                    increaseFeedRate();
                }
                else if (strcmp(serialBuffer, decreaseFeedRatePrefix) == 0) { // "$frd"
                    decreaseFeedRate();
                }
                else if (strcmp(serialBuffer, increaseDepthOfCutPrefix) == 0) { // "$dci"
                    increaseDepthOfCut();
                }
                else if (strcmp(serialBuffer, decreaseDepthOfCutPrefix) == 0) { // "$dcd"
                    decreaseDepthOfCut();
                }
                lastCommandTime = millis();
            }
            else if (strcmp(serialBuffer, manualUpPrefix) == 0) {     // "$up"
                manualPositioning(true, false);
            }
            else if (strcmp(serialBuffer, manualDownPrefix) == 0) {   // "$down"
                manualPositioning(false, true);
            }

            // Reset buffer for next message
            bufferIndex = 0;
        }
        else
        {
            if (bufferIndex < sizeof(serialBuffer) - 1)
            {
                serialBuffer[bufferIndex++] = c;
            }
        }
    }
}


void sendAllSerial()
{
  if (getRigState() == REAMING)
  {
    serialMeasure = 1;
  }
  else
  {
    serialMeasure = -1;
  }

  sendSerialMessage(startStoringPrefix, serialMeasure);
  sendSerialMessage(currentPosistionPrefix, getStepperPosition());
  sendSerialMessage(rpmPrefix, getRPM());
  sendSerialMessage(feedRatePrefix, getFeedRate());
  sendSerialMessage(depthOfCutPrefix, getDepthOfCut());
  
}

void sendSerialMessage(String prefix, unsigned long value)
{
  Serial.print(prefix);
  Serial.print(value);   
  Serial.print(postfix);
}


void sendSerialMessage(String prefix, int value)
{
  Serial.print(prefix);
  Serial.print(value);   
  Serial.print(postfix);
}


void sendSerialMessage(String prefix, float value)
{
  Serial.print(prefix);
  Serial.print(value);   
  Serial.print(postfix);
}


void sendSerialMessage(String prefix, long int value)
{
  Serial.print(prefix);
  Serial.print(value);   
  Serial.print(postfix);
}
