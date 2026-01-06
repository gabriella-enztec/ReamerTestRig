#ifndef SEQUENCEMANAGER_H
#define SEQUENCEMANAGER_H

#pragma once

enum SequenceID {SEQ_NONE, SEQ_ZERO, SEQ_REAM};

void updateSequences();
void setActiveSequence(SequenceID seq);
SequenceID getActiveSequence();

#endif