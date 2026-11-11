
/*
 * sequenceManager.cpp
 *
 * File Overview:
 *  - Manages all machine sequences, including zeroing, reaming, and manual motion.
 *  - Tracks the currently active sequence and handles transitions between sequences.
 *  - Ensures manual movement is only allowed when appropriate.
 *
 * Key Variables:
 *  - activeSequence: The currently active sequence (SEQ_NONE, SEQ_ZERO, SEQ_REAM, SEQ_UP, SEQ_DOWN)
 *  - previousSequence: Stores the previously active sequence (currently unused)
 *  - nodeEntered: Flag used for sequence state transitions (currently unused)
 *
 * Functions:
 *  - setActiveSequence(): Sets the currently active sequence.
 *  - getActiveSequence(): Returns the currently active sequence.
 *  - canManualMove(): Returns true if manual movement is allowed.
 *  - requestManualMove(): Requests a manual motion direction, updates sequence accordingly.
 *  - updateSequences(): Executes the active sequence; should be called periodically.
 */

#include <Arduino.h>
#include "HeaderFiles/sequenceManager.h"
#include "HeaderFiles/zero.h"
#include "HeaderFiles/ream.h"
#include "HeaderFiles/motion_controller.h"
#include "HeaderFiles/settings.h"


// ----------- SEQUENCE STATE TRACKING -----------
static SequenceID activeSequence = SEQ_NONE;    // Currently active sequence
static SequenceID previousSequence = SEQ_NONE;  // Previous sequence (for tracking transitions)
static bool nodeEntered = false;                // Flag for sequence node entry 


/*
 * setActiveSequence()
 *
 * Sets the currently active sequence.
 * Resets nodeEntered flag when switching sequences.
 */
void setActiveSequence(SequenceID seq)
{
    nodeEntered = false;
    activeSequence = seq;
}


/*
 * getActiveSequence()
 *
 * Returns the currently active sequence.
 */
SequenceID getActiveSequence()
{
    return activeSequence;
}


/*
 * canManualMove()
 *
 * Determines whether manual movement is allowed.
 * Manual motion is only allowed if no active sequence or UP/DOWN manual sequence is active.
 */
bool canManualMove()
{
    // Only allow manual movement when no sequence is active
    return (activeSequence == SEQ_NONE || activeSequence == SEQ_UP || activeSequence == SEQ_DOWN);
}


/*
 * requestManualMove()
 *
 * Requests a manual movement direction from the buttons layer.
 * Updates the active sequence based on requested direction.
 * Stops movement if STOP is requested.
 */
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


/*
 * updateSequences()
 *
 * Periodically updates and executes the currently active sequence.
 * Handles zeroing, reaming, and manual motion sequences.
 * Transitions to SEQ_NONE when sequences finish.
 */
void updateSequences()
{
    switch(activeSequence)
    {
        case SEQ_ZERO:
            // Zeros the position of the reamer
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
            // Reaming Sequence
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
            // Manual Movement Up
            manualMovement(UP, getManualFeedRate());
            break;
        
        case SEQ_DOWN:
            // Manual Movement Down
            manualMovement(DOWN, getManualFeedRate());
            break;

        default:
            // Saftey Stop if activeSequence Invalid
            manualMovement(STOP, getManualFeedRate());
            break;
    }
}
