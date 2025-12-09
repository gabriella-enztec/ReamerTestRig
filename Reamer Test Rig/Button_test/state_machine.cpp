#include "state_machine.h"

TestRigState currentState;

void state_machine_init() {
    currentState = IDLE;
}

void setIdle() { currentState = IDLE; }
void setManual() { currentState = MANUAL; }
void setPositioning() { currentState = POSITIONING; }
void setReaming() { currentState = REAMING; }
void setRetracting() { currentState = RETRACTING; }

TestRigState getRigState() {
    return currentState;
}


