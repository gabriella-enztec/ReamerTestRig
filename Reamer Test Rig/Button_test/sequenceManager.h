#ifndef SEQUENCEMANAGER_H
#define SEQUENCEMANAGER_H

#include "motion_controller.h"

enum SequenceID {SEQ_NONE, SEQ_ZERO, SEQ_REAM, SEQ_UP, SEQ_DOWN};

void updateSequences();
void setActiveSequence(SequenceID seq);
SequenceID getActiveSequence();

void requestManualMove(Direction dir);

#endif