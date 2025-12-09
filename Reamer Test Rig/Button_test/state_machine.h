#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

enum TestRigState
{
  IDLE,
  MANUAL,
  POSITIONING,
  REAMING,
  RETRACTING
};

void init();
void setIdle();
void setManual();
void setPositioning();
void setReaming();
void setRetracting();
void state_machine_init();
TestRigState getRigState();

#endif







