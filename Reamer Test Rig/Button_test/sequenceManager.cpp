
#include "sequenceManager.h"
#include "zero.h"
#include "ream.h"

#include <Arduino.h>

static SequenceID activeSequence = SEQ_NONE;
static bool nodeEntered = false;

void setActiveSequence(SequenceID seq)
{
    // Reset node state for the new sequence
    nodeEntered = false;

    // Set the active sequence
    activeSequence = seq;
}

SequenceID getActiveSequence()
{
    return activeSequence;
}


void updateSequences()
{
    switch(activeSequence)
    {
        case SEQ_ZERO:
            Serial.println("SEQ_ZERO");
            zeroSequenceUpdate();
            if(zeroFinished())
            {
              setActiveSequence(SEQ_NONE); 
            }
            break;

        case SEQ_REAM:
            Serial.println("SEQ_REAM");
            reamSequence();
            if (reamFinished())
            {
              setActiveSequence(SEQ_NONE); 
            }
            break;

        default:
            break;
    }
}
