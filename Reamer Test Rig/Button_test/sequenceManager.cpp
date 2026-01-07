
#include "sequenceManager.h"
#include "zero.h"
#include "ream.h"
#include "motion_controller.h"
#include "settings.h"

#include <Arduino.h>

static SequenceID activeSequence = SEQ_NONE;
static SequenceID previousSequence = SEQ_NONE;
static bool nodeEntered = false;

void setActiveSequence(SequenceID seq)
{
    nodeEntered = false;
    activeSequence = seq;
}


SequenceID getActiveSequence()
{
    return activeSequence;
}


bool canManualMove()
{
    // Only allow manual movement when no sequence is active
    return (activeSequence == SEQ_NONE || activeSequence == SEQ_UP || activeSequence == SEQ_DOWN);
}

// NEW: Request manual movement (called from buttons layer)
void requestManualMove(Direction dir)
{
    if (!canManualMove())
    {
        return;
    }
    
    // Update sequence state
    if (dir == STOP)
    {
        activeSequence = SEQ_NONE;
    }
    else if (dir == UP)
    {
        activeSequence = SEQ_UP;
    }
    else if (dir == DOWN)
    {
        activeSequence = SEQ_DOWN;
    }
}


void updateSequences()
{
    switch(activeSequence)
    {
        case SEQ_ZERO:
            if(zeroFinished())
            {
                setActiveSequence(SEQ_NONE);
            }
            else
            {
                zeroSequenceUpdate();
            }
            break;

        case SEQ_REAM:
            if (reamFinished())
            {
                setActiveSequence(SEQ_NONE);
            }
            else
            {
                reamSequence();
            }
            break;

        case SEQ_UP:
            manualMovement(UP, getManualFeedRate());
            break;
        
        case SEQ_DOWN:
            manualMovement(DOWN, getManualFeedRate());
            break;

        default:
            manualMovement(STOP, getManualFeedRate());
            break;
    }
}
