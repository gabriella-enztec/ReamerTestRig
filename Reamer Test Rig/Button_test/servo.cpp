#include "settings.h"

#define SERVO_SWITCH_PIN 3 // PWM
#define OP_AMP_PIN 9 // PWM

#include <Arduino.h>


#define MAX_RPM 1000.0
#define MIN_PWM_VOLTAGE 2.12



void OnServo(bool on)
{
  digitalWrite(SERVO_SWITCH_PIN, on ? HIGH : LOW);
}


void servo_init()
{
  pinMode(SERVO_SWITCH_PIN, OUTPUT);
  pinMode(OP_AMP_PIN, OUTPUT);

  OnServo(false);
}


uint8_t rpmToPWM(float rpm)
{
    if (rpm < 0) rpm = 0;
    if (rpm > MAX_RPM) rpm = MAX_RPM;

    float voltage = MIN_PWM_VOLTAGE * (1.0 - (rpm / MAX_RPM));
    return (uint8_t)((voltage / 5.0) * 255.0);
}


void setServoSpeed(float rpm)
{
  analogWrite(OP_AMP_PIN, rpmToPWM(rpm));
}



