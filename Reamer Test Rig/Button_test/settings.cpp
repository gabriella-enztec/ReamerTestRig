// settings.cpp
#include "settings.h"

// ----------- CONFIGURABLE LIMITS -----------
const int RPM_MIN = 0;
const int RPM_MAX = 1000;
const float FEED_MIN = 0;
const float FEED_MAX = 100;
const float DOC_MIN = 0;
const float DOC_MAX = 100;

// ----------- DEAFULT REAMING SETTINGS -----------
const int RPM_REAMING_DEAFULT = 270;
const float FEED_REAMING_DEAFULT = 0.5;
const float DOC_REAMING_DEAFULT = 4;

// ----------- DEAFULT DRILLING SETTINGS -----------
const int RPM_DRILL_DEAFULT = 270;
const float FEED_DRILL_DEAFULT = 2;
const float DOC_DRILL_DEAFULT = 55;

// ----------- CURRENT SETTINGS -----------
int RPM = 270;
float FEED = 0.5;
float DOC = 4;


void initSettings() {
  loadDefaultReamingSettings();
}

// ========== RPM ==========
void increaseRPM() {
    if (RPM < RPM_MAX)
        RPM += 10;
}

void decreaseRPM() {
    if (RPM > RPM_MIN)
        RPM -= 10;
}

// ========== FEED RATE ==========
void increaseFeedRate() {
    if (FEED < FEED_MAX)
        FEED += 0.1;
}

void decreaseFeedRate() {
    if (FEED > FEED_MIN)
        FEED -= 0.1;
}

// ========== DEPTH OF CUT ==========
void increaseDepthOfCut() {
    if (DOC < DOC_MAX)
        DOC += 0.2;
}

void decreaseDepthOfCut() {
    if (DOC > DOC_MIN)
        DOC -= 0.2;
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
