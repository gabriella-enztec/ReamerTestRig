
/*
#include "servo.h"

#define PWM_PIN_ON 1
#define PWM_PIN_RPM 2


void servo_init()
{
  pinMode(PWM_PIN_ON, OUTPUT);
  pinMode(PWM_PIN_RPM, OUTPUT);
}

void setVoltage(int pin, float volts)
{
  volts = constrain(volts, 0.0, 5.0);
  int pwm = (volts / 5.0) * 255;
  analogWrite(pin, pwm);
}

void servoOff()
{
  setVoltage(PWM_PIN_ON, 0);
}


void servoOn()
{
  setVoltage(PWM_PIN_ON, 5);
}


void setServoRPM(float rpm)
{
  
}
*/
