
/*
 * settings.cpp
 *
 * File Overview:
 *  - Stores and manages all machine operating parameters, including RPM, feed rate,
 *    depth of cut (DOC), and manual/rapid feed speed.
 *  - Provides functions to increase or decrease each parameter within safe limits.
 *  - Supports default presets for reaming and drilling operations.
 *  - Includes getter and setter functions to access current values and load readings.
 *  - Initializes default settings on startup.
 *
 * Design Notes:
 *  - Limits and step increments are configurable via constants at the top of the file.
 *  - Current values are stored in global variables and updated via functions.
 *  - Ensures all values stay within defined minimum and maximum limits.
 *  - Separation of default presets and current settings allows easy switching between operations.
 */

#include "settings.h"

// ----------- CONFIGURABLE LIMITS -----------
const float RPM_MIN = 0;
const float RPM_MAX = 1000;
const float FEED_MIN = 0;
const float FEED_MAX = 100;
const float DOC_MIN = 0;
const float DOC_MAX = 100;
const float RAPID_SPEED_MIN = 0;
const float RAPID_SPEED_MAX = 20;

const float RPM_iter = 10;
const float FEED_iter = 0.1;
const float DOC_iter = 0.5;
const float RAPID_SPEED_iter = 0.5; 

// ----------- DEAFULT REAMING SETTINGS -----------
const float RPM_REAMING_DEAFULT = 270;
const float FEED_REAMING_DEAFULT = 0.5;
const float DOC_REAMING_DEAFULT = 4;

// ----------- DEAFULT DRILLING SETTINGS -----------
const int RPM_DRILL_DEAFULT = 270;
const float FEED_DRILL_DEAFULT = 3;
const float DOC_DRILL_DEAFULT = 65;

// ----------- CURRENT SETTINGS -----------
float RPM = 500;
float FEED = 0.5;
float DOC = 4;

float RAPID_SPEED = 5;
float LOAD = 0;


/*
 * initSettings()
 *
 * Initializes the system settings on startup.
 * Loads the default reaming parameters.
 */
void initSettings() {
  loadDefaultReamingSettings();
}


// ========== RPM ==========
/*
 * increaseRPM()
 *
 * Increases the current RPM by the defined increment.
 * Clamps the value to RPM_MAX to prevent exceeding limits.
 */
void increaseRPM() 
{
  if (RPM + RPM_iter > RPM_MAX)
  {
    RPM = RPM_MAX;
  }
  else
  {
    RPM += RPM_iter;
  }
}


/*
 * decreaseRPM()
 *
 * Decreases the current RPM by the defined increment.
 * Clamps the value to RPM_MIN to prevent going below limits.
 */
void decreaseRPM() 
{
  if (RPM - RPM_iter < RPM_MIN)
  {
    RPM = RPM_MIN;
  }
  else
  {
    RPM -= RPM_iter;
  }
}

// ========== FEED RATE ==========
/*
 * increaseFeedRate()
 *
 * Increases the current feed rate by the defined increment.
 * Clamps the value to FEED_MAX to prevent exceeding limits.
 */
void increaseFeedRate() 
{
  if (FEED + FEED_iter > FEED_MAX)
  {
    FEED = FEED_MAX;
  }
  else
  {
    FEED += FEED_iter;
  }
}


/*
 * decreaseFeedRate()
 *
 * Decreases the current feed rate by the defined increment.
 * Clamps the value to FEED_MIN to prevent going below limits.
 */
void decreaseFeedRate() 
{
  if (FEED - FEED_iter < FEED_MIN)
  {
    FEED = FEED_MIN;
  }
  else
  {
    FEED -= FEED_iter;
  }
}


// ========== DEPTH OF CUT ==========
/*
 * increaseDepthOfCut()
 *
 * Increases the current depth of cut (DOC) by the defined increment.
 * Clamps the value to DOC_MAX to prevent exceeding limits.
 */
void increaseDepthOfCut() 
{
  if (DOC + DOC_iter > DOC_MAX)
  {
    DOC = DOC_MAX;
  }
  else
  {
    DOC += DOC_iter;
  }
}


/*
 * decreaseDepthOfCut()
 *
 * Decreases the current depth of cut (DOC) by the defined increment.
 * Clamps the value to DOC_MIN to prevent going below limits.
 */
void decreaseDepthOfCut() 
{
  if (DOC - DOC_iter < DOC_MIN)
  {
    DOC = DOC_MIN;
  }
  else
  {
    DOC -= DOC_iter;
  }
}


/*
 * increaseManualFeedRate()
 *
 * Increases the manual feed/rapid speed by the defined increment.
 * Clamps the value to RAPID_SPEED_MAX to prevent exceeding limits.
 */
void increaseManualFeedRate()
{
  if (RAPID_SPEED + RAPID_SPEED_iter >= RAPID_SPEED_MAX)
  {
    RAPID_SPEED = RAPID_SPEED_MAX;
  }
  else
  {
    RAPID_SPEED += RAPID_SPEED_iter;
  }
}


/*
 * decreaseManualFeedRate()
 *
 * Decreases the manual feed/rapid speed by the defined increment.
 * Clamps the value to RAPID_SPEED_MIN to prevent going below limits.
 */
void decreaseManualFeedRate()
{
  if (RAPID_SPEED - RAPID_SPEED_iter <= RAPID_SPEED_MIN)
  {
    RAPID_SPEED = RAPID_SPEED_MIN;
  }
  else
  {
    RAPID_SPEED -= RAPID_SPEED_iter;
  }
}


// ========== DEFAULT PRESETS ==========

/*
 * loadDefaultReamingSettings()
 *
 * Loads the default reaming operation parameters into current settings.
 */
void loadDefaultReamingSettings() {
    RPM = RPM_REAMING_DEAFULT;
    FEED = FEED_REAMING_DEAFULT;
    DOC = DOC_REAMING_DEAFULT;
}


/*
 * loadDefaultDrillingSettings()
 *
 * Loads the default drilling operation parameters into current settings.
 */
void loadDefaultDrillingSettings() {
    // You can use different defaults here
    RPM = RPM_DRILL_DEAFULT;
    FEED = FEED_DRILL_DEAFULT;
    DOC = DOC_DRILL_DEAFULT;
}


/*
 * getRPM()
 *
 * Returns the current RPM value.
 */
float getRPM()
{
  return RPM;
}


/*
 * getFeedRate()
 *
 * Returns the current feed rate value.
 */
float getFeedRate()
{
  return FEED;
}


/*
 * getDepthOfCut()
 *
 * Returns the current depth of cut (DOC) value.
 */
float getDepthOfCut()
{
  return DOC;
}


/*
 * setLoad()
 *
 * Updates the current load value.
 */
void setLoad(float load)
{
  LOAD = load;
}


/*
 * getLoad()
 *
 * Returns the current load value.
 */
float getLoad()
{
  return LOAD;
}


/*
 * getManualFeedRate()
 *
 * Returns the current manual feed / rapid speed.
 */
float getManualFeedRate()
{
  return RAPID_SPEED;
}
