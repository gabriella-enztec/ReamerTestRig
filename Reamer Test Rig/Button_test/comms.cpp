
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

const int timepositionpast = 50;
int sincelastposition = 0;

// Serial command timing
unsigned long lastCommandTime = 0;
const unsigned long debounceDelay = 50;

// Prefix Tx (Transmitting)
const String currentPosistionPrefix = "\$p";
const String startStoringPrefix    = "\$s";
const String rpmPrefix             = "\$r";
const String feedRatePrefix        = "\$f";
const String depthOfCutPrefix      = "\$d";
const String LoadPrefix            = "\$l";
const String rapidSpeedPrefix      = "\$o";

// Prefix Rx (Receiving)
const char startReamingPrefix           = 'a';
const char zeroPrefix                   = 'b';
const char manualUpPrefix               = 'c';
const char manualDownPrefix             = 'd';
const char deafultReamingSettingPrefix  = 'e';
const char deafultDrillingSettingPrefix = 'f';
const char increaseRPMPrefix            = 'g';
const char decreaseRPMPrefix            = 'h';
const char increaseFeedRatePrefix       = 'i';
const char decreaseFeedRatePrefix       = 'j';
const char increaseDepthOfCutPrefix     = 'k';
const char decreaseDepthOfCutPrefix     = 'l';
const char loadPrefix                   = 'm';
const char increaseManualFeedRatePrefix = 'n';
const char decreaseManualFeedRatePrefix = 'o';

// Postfix
const String postfix = "\\n";
const char postfix2 = 'x';



void serial_init()
{
    // Initialize serial communication
  Serial.begin(9600, SERIAL_8N1);
  input.reserve(INPUT_BUFFER_LEN);
}

// Buffer for incoming serial command
char serialBuffer[20];
uint8_t bufferIndex = 0;
bool loading = false;


void checkSerialCommand()
{
    while (Serial.available())
    {
        char c = Serial.read();

        if (!loading && c == loadPrefix)
        {
            loading = true;
            bufferIndex = 0;           // reset buffer
            continue;
        }

        // If we are currently loading a message
        if (loading)
        {
            // Check postfix (end of message)
            if (c == postfix2)
            {
                serialBuffer[bufferIndex] = '\0'; // null-terminate
                processMessage(serialBuffer);     // handle the message
                loading = false;                  // reset state
                bufferIndex = 0;
                continue;
            }

            // Store character safely into buffer
            if (bufferIndex < sizeof(serialBuffer) - 1)
            {
                serialBuffer[bufferIndex++] = c;
            }
            else
            {
                // Buffer overflow → reset
                loading = false;
                bufferIndex = 0;
            }
        }
        // Debounce
        if (millis() - lastCommandTime > debounceDelay)
        {
            switch (c)
            {
                case startReamingPrefix:
                    reamCycle();
                    break;
                case zeroPrefix:
                    zero();
                    break;
                case deafultReamingSettingPrefix:
                    loadDefaultReamingSettings();
                    break;
                case deafultDrillingSettingPrefix:
                    loadDefaultDrillingSettings();
                    break;
                case increaseRPMPrefix:
                    increaseRPM();
                    break;
                case decreaseRPMPrefix:
                    decreaseRPM();
                    break;
                case increaseFeedRatePrefix:
                    increaseFeedRate();
                    break;
                case decreaseFeedRatePrefix:
                    decreaseFeedRate();
                    break;
                case increaseDepthOfCutPrefix:
                    increaseDepthOfCut();
                    break;
                case decreaseDepthOfCutPrefix:
                    decreaseDepthOfCut();
                    break;
                case increaseManualFeedRatePrefix:
                    increaseManualFeedRate();
                    break;
                case decreaseManualFeedRatePrefix:
                    decreaseManualFeedRate();
                    break;
                default:
                    break;
            }
            lastCommandTime = millis();
        }

        else if (c == manualUpPrefix) {     // "$up"
            manualPositioning(true, false);
        }
        else if (c == manualDownPrefix) {   // "$down"
            manualPositioning(false, true);
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
    sendSerialMessage(currentPosistionPrefix, getStepperPositionmm());
    sendSerialMessage(rpmPrefix, getRPM());
    sendSerialMessage(feedRatePrefix, getFeedRate());
    sendSerialMessage(depthOfCutPrefix, getDepthOfCut());
    sendSerialMessage(LoadPrefix, getLoad());
    sendSerialMessage(rapidSpeedPrefix, getManualFeedRate());
}

void sendPosistion()
{
    if (millis() - sincelastposition >= timepositionpast)
    {
        sendSerialMessage(currentPosistionPrefix, getStepperPositionmm());
        sincelastposition = millis();
    }
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


void processMessage(char* msg)
{
    // Convert string → float
    float value = atof(msg);
    setLoad(value);

}
