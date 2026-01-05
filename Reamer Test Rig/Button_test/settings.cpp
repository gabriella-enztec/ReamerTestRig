// settings.cpp
#include "settings.h"

// ----------- CONFIGURABLE LIMITS -----------
const int RPM_MIN = 0;
const int RPM_MAX = 1000;
const float FEED_MIN = 0;
const float FEED_MAX = 100;
const float DOC_MIN = 0;
const float DOC_MAX = 100;
const float RAPID_SPEED_MIN = 0;
const float RAPID_SPEED_MAX = 20;

const int RPM_iter = 10;
const float FEED_iter = 0.1;
const float DOC_iter = 0.5;
const float RAPID_SPEED_iter = 0.5; 

// ----------- DEAFULT REAMING SETTINGS -----------
const int RPM_REAMING_DEAFULT = 270;
const float FEED_REAMING_DEAFULT = 0.5;
const float DOC_REAMING_DEAFULT = 4;

// ----------- DEAFULT DRILLING SETTINGS -----------
const int RPM_DRILL_DEAFULT = 270;
const float FEED_DRILL_DEAFULT = 3;
const float DOC_DRILL_DEAFULT = 65;

// ----------- CURRENT SETTINGS -----------
int RPM = 270;
float FEED = 0.5;
float DOC = 4;

float RAPID_SPEED = 5;
float LOAD = 0;


void initSettings() {
  loadDefaultReamingSettings();
}


// ========== RPM ==========
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
void loadDefaultReamingSettings() {
    RPM = RPM_REAMING_DEAFULT;
    FEED = FEED_REAMING_DEAFULT;
    DOC = DOC_REAMING_DEAFULT;
}


void loadDefaultDrillingSettings() {
    // You can use different defaults here
    RPM = RPM_DRILL_DEAFULT;
    FEED = FEED_DRILL_DEAFULT;
    DOC = DOC_DRILL_DEAFULT;
}


int getRPM()
{
  return RPM;
}


float getFeedRate()
{
  return FEED;
}


float getDepthOfCut()
{
  return DOC;
}


void setLoad(float load)
{
  LOAD = load;
}


float getLoad()
{
  return LOAD;
}


float getManualFeedRate()
{
  return RAPID_SPEED;
}
