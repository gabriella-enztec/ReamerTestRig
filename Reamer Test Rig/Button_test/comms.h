#ifndef COMMS_H_
#define COMMS_H_

#include <Arduino.h>

void checkSerialCommand();
void serial_init();
void sendAllSerial();

/**
void sendSerialMessage(const char* prefix, unsigned long value);
*/
void sendSerialMessage(String prefix, int value);

/*
void sendSerialMessage(const char* prefix, float value);
void sendSerialMessage(const char* prefix, long int value);
*/
#endif