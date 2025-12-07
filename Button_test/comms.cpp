#include "comms.h"

// Input buffer for incoming commands from Dewesoft

String input = "";
bool input_complete = false;

//Takes a command string beginning with set and sets the
//passed parameter variable. Command string must be of the 
//format 'set var input_int\n' and have a maximum length of
//20 chars.
void setTime(unsigned long* for_t) {
  *for_t = input.substring(2,6).toInt();
}

// serialEvent is automatically called at the end of loop() if
// a serial event takes place (ie Rx pin receives information from
// Dewesoft. One char is grabbed per call.
/*
void serialEvent(){
  if(Serial.available()){
    char input_char = (char)Serial.read();
    input += input_char;
    if (input_char == '0') {
      input_complete = true;
    }
  }
}
*/
