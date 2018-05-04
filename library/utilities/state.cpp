/************************************************************************
*    FILE NAME:       state.cpp
*
*    DESCRIPTION:     Class for state machine handling
************************************************************************/

// Physical component dependency
#include <utilities/state.h>

// Standard lib dependencies
#include <assert.h>

/************************************************************************
*    DESC:  constructor
************************************************************************/
CState::CState() :
    pState(nullptr),
    level(0)
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CState::~CState()
{
}


/************************************************************************
*    DESC:  Initilize the state class
************************************************************************/
void CState::initilize()
{
    assert(pState == nullptr && "Persistant state pointer already initilized!");

    // Check that we have not over run the queue
    assert(perState.state.size() == 0 && "Queue used before being initilized!");

    // Add the first state to the queue
    perState.state.push_back(0);

    // Init the pointer to the persistant class
    pState = &perState;
}


/************************************************************************
*    DESC:  Returns the current state value
************************************************************************/
CState::operator int() const
{
    // Sanity checks
    assert(pState != nullptr && "Persistant state pointer NOT initilized!");

    // Check that we have not over run the queue
    assert(level < pState->state.size() && "Level exceeds queue size!");

    // Just return the current states value for this level.
    return pState->state[level];
}


/************************************************************************
*    DESC:  Set this state's current value.
************************************************************************/
int CState::operator = (int newState)
{
    // Sanity checks
    assert(pState != nullptr && "Persistant state pointer NOT initilized!");

    // Check that we have not over run the queue
    assert(level < pState->state.size() && "Level exceeds queue size!");

    // Don't do anything if it's the same state.
    if( pState->state[level] == newState )
        return newState;

    // Change our new state value for this level.
    pState->state[level] = newState;

    // Destroy any access to substates
    pState->level = level;

    // Pop any substates
    if( level+1 < pState->state.size() )
    {
        assert(pState->state.size() != 0 && "Possible run away sub state! Sub state queue 0 before pop!!");
        pState->state.pop_back();
    }

    // Return out new state value.
    return newState;
}


/************************************************************************
*    DESC:  Create a substate by pushing onto the stack a new state value of zero.
************************************************************************/
CState CState::newSubState() const
{
    // Sanity checks
    assert(pState != nullptr && "Persistant state pointer NOT initilized!");

    // See if we need to add a new state
    if( level == pState->level )
    {
        // Increment the current state ptr.
        ++pState->level;

        // Create a new substate.
        pState->state.push_back(0);

        // Guard against a run away sub state queue
        assert(pState->state.size() < 50 && "Possible run away sub state! Sub state queue of 50!!");

        // Check that the new level is the last index in the queue
        assert( pState->level+1 == pState->state.size() && "Level and queue size out of sync!!");
    }

    // Create the state object referencing this substate and return it.
    CState state;

    // Equals the current level plus one.
    state.level = level+1;

    // Copy the ptr to nv object.
    state.pState = pState;

    // Reutrn our new substate for use.
    return state;
}
