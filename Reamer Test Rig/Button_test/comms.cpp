
/*
 * comms.cpp
 *
 * File Overview:
 *  - Handles all serial communication between the embedded controller
 *    and an external host (e.g. DEWESoft).
 *  - Implements a lightweight command protocol for receiving control
 *    commands and transmitting system telemetry.
 *  - Acts as the primary interface between software control and the user.
 *
 * Communication Design:
 *  - Single-character commands are used for immediate control actions.
 *  - Framed messages (prefix + payload + postfix) are used for numeric data.
 *  - All communication is non-blocking and processed incrementally.
 */

#include "comms.h"
#include "buttons.h"
#include "ream.h"
#include "stepper_movement.h"
#include "settings.h"
#include "sequenceManager.h"

#include <Arduino.h>

// -----------------------------------------------------------------------------
// Serial configuration and buffers
// -----------------------------------------------------------------------------

// Serial communication settings
#define INPUT_BUFFER_LEN 64     // Reserved size for Arduino String input

String input;
int serialMeasure = 0;

// Position telemetry timing
const int timepositionpast = 50;    // ms between position updates
int sincelastposition = 0;

// Serial command timing
unsigned long lastCommandTime = 0;
const unsigned long debounceDelay = 50;


// -----------------------------------------------------------------------------
// Transmit (Tx) message prefixes
// -----------------------------------------------------------------------------

const String currentPosistionPrefix = "\$p";   // Current position (mm)
const String startStoringPrefix    = "\$s";   // Reaming active flag
const String rpmPrefix             = "\$r";   // Spindle RPM
const String feedRatePrefix        = "\$f";   // Feed rate
const String depthOfCutPrefix      = "\$d";   // Depth of cut
const String LoadPrefix            = "\$l";   // Load value
const String rapidSpeedPrefix      = "\$o";   // Manual feed rate
const String settingsNotifPrefix   = "\$q";   // Settings change indicator


// -----------------------------------------------------------------------------
// Receive (Rx) command prefixes
// -----------------------------------------------------------------------------

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


// -----------------------------------------------------------------------------
// Message postfix definitions
// -----------------------------------------------------------------------------

const String postfix = "\\n";   // End-of-line for transmitted messages
const char postfix2 = 'x';    // End-of-message marker for framed input


// -----------------------------------------------------------------------------
// Serial initialisation
// -----------------------------------------------------------------------------


/*
 * serial_init()
 *
 * Configures the serial interface and reserves memory
 * for incoming string data.
 */
void serial_init()
{
  Serial.begin(9600, SERIAL_8N1);
  input.reserve(INPUT_BUFFER_LEN);
}


// -----------------------------------------------------------------------------
// Incoming serial command buffer
// -----------------------------------------------------------------------------

char serialBuffer[20];     // Buffer for framed numeric messages
uint8_t bufferIndex = 0;  // Current buffer write index
bool loading = false;     // Indicates active message reception

// -----------------------------------------------------------------------------
// Serial receive handler
// -----------------------------------------------------------------------------


/*
 * checkSerialCommand()
 *
 * Reads and processes incoming serial data.
 * Supports:
 *  - Single-character command inputs
 *  - Framed numeric messages (load value)
 *
 * Must be called periodically by the main scheduler.
 */
void checkSerialCommand()
{
    while (Serial.available())
    {
        char c = Serial.read();
        
        // Start framed message reception
        if (!loading && c == loadPrefix)
        {
            loading = true;
            bufferIndex = 0;           // reset buffer
            continue;
        }

        // Process framed numeric message
        if (loading)
        {
            // End-of-message detected
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

        // Debounced single-character command handling
        if (millis() - lastCommandTime > debounceDelay)
        {
            switch (c)
            {
                case startReamingPrefix:
                    setActiveSequence(SEQ_REAM);
                    break;

                case zeroPrefix:
                    setActiveSequence(SEQ_ZERO);
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
    }
}


// -----------------------------------------------------------------------------
// Serial transmit functions
// -----------------------------------------------------------------------------

/*
 * sendAllSerial()
 *
 * Transmits all relevant system telemetry to the host.
 * Intended for periodic execution.
 */
void sendAllSerial()
{
    sendSerialMessage(startStoringPrefix, isReaming());
    sendSerialMessage(currentPosistionPrefix, getStepperPositionmm());
    sendSerialMessage(rpmPrefix, getRPM());
    sendSerialMessage(feedRatePrefix, getFeedRate());
    sendSerialMessage(depthOfCutPrefix, getDepthOfCut());
    sendSerialMessage(LoadPrefix, getLoad());
    sendSerialMessage(rapidSpeedPrefix, getManualFeedRate());
    sendSerialMessage(settingsNotifPrefix, 'A');
}


/*
 * sendPosition()
 *
 * Periodically transmits the current position only.
 */
void sendPosistion()
{
    if (millis() - sincelastposition >= timepositionpast)
    {
        sendSerialMessage(currentPosistionPrefix, getStepperPositionmm());
        sincelastposition = millis();
    }
}


// -----------------------------------------------------------------------------
// Serial message helpers
// -----------------------------------------------------------------------------

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


void sendSerialMessage(String prefix, const char* value)
{
  Serial.print(prefix);
  Serial.print(value);   
  Serial.print(postfix);
}


// -----------------------------------------------------------------------------
// Framed message processing
// -----------------------------------------------------------------------------

/*
 * processMessage()
 *
 * Handles framed numeric input messages.
 * Currently used to receive load values from the host.
 *
 * Parameters:
 *  - msg : Null-terminated string containing numeric data
 */
void processMessage(char* msg)
{
    // Convert string → float
    float value = atof(msg);
    setLoad(value);

}
