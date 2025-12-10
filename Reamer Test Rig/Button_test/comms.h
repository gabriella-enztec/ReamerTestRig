#ifndef COMMS_H_
#define COMMS_H_

#include <Arduino.h>

void checkSerialCommand();
void serial_init();
void sendAllSerial();


void sendSerialMessage(String prefix, unsigned long value);
void sendSerialMessage(String prefix, int value);
void sendSerialMessage(String prefix, float value);
void sendSerialMessage(String prefix, long int value);

#endif