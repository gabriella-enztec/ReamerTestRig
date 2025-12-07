#ifndef comms
#define comms

#include "Arduino.h"

#define REAM 0

extern String input;
extern bool input_complete;

// Input buffer for incoming commands from Dewesoft
#define INPUT_BUFFER_LEN 20

void setTime(unsigned long* for_t);
void serialEvent();

char getInput();

#endif
