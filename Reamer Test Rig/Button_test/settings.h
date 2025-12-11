// settings.h
#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>

void initSettings();

// RPM control
void increaseRPM();
void decreaseRPM();

// Feed rate control
void increaseFeedRate();
void decreaseFeedRate();

// Depth of cut control
void increaseDepthOfCut();
void decreaseDepthOfCut();

// Reset functions
void loadDefaultReamingSettings();
void loadDefaultDrillingSettings();

// Get functions
int getRPM();
float getFeedRate();
float getDepthOfCut();
float getLoad();

void setLoad(float load);

#endif
