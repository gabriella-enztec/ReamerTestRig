// settings.cpp
#include "settings.h"

// ----------- CONFIGURABLE LIMITS -----------
const int RPM_MIN = 100;
const int RPM_MAX = 3000;
const float FEED_MIN = 10;
const float FEED_MAX = 200;
const float DOC_MIN = 1;
const float DOC_MAX = 10;

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
        FEED += 5;
}

void decreaseFeedRate() {
    if (FEED > FEED_MIN)
        FEED -= 5;
}

// ========== DEPTH OF CUT ==========
void increaseDepthOfCut() {
    if (DOC < DOC_MAX)
        DOC += 1;
}

void decreaseDepthOfCut() {
    if (DOC > DOC_MIN)
        DOC -= 1;
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
