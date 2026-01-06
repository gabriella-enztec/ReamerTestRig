
#include "sequenceManager.h"
#include "zero.h"
#include "ream.h"

#include <Arduino.h>

static SequenceID activeSequence = SEQ_NONE;
static SequenceID previousSequence = SEQ_NONE;
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
            zeroSequenceUpdate();
            //previousSequence = SEQ_ZERO;
            if(zeroFinished())
            {
                setActiveSequence(SEQ_NONE);
                //previousSequence =  SEQ_NONE;
            }
            break;

        case SEQ_REAM:
            reamSequence();
            //previousSequence = SEQ_REAM;
            if (reamFinished())
            {
                setActiveSequence(SEQ_NONE);
                //previousSequence = SEQ_ZERO; 
            }
            break;

        default:
            break;
    }
}
