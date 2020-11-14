/************************************************************************
*    FILE NAME:       state.h
*
*    DESCRIPTION:     Class for state machine handling
************************************************************************/

#pragma once

// Standard lib dependencies
#include <vector>

class CState
{
public:

    // Construct a state object which can do nothing until opened.
    CState();

    // Destruct our state object.
    ~CState();

    // Initilize the state class
    void initilize();

    // Returns the current state value for the substate.
    operator int() const;

    // Set this state's current value. This will change the current state value
    // for this states position in the state stack and will also destroy any
    // substates farther down the stack.  Changing a state higher up the stack
    // essentially destroyes or transitions out of substates.
    int operator = (int newState);

    // Create a substate by pushing onto the stack a new state value of zero.
    // This returns a new state varible which references the new substack within
    // the state stack.  Use this new state returned to use and change the
    // substate you have created.
    CState newSubState () const;

protected:

    // Class to hold the persistant state info
    class PersistantState
    {
    public:

        PersistantState() : level(0)
        {}

        // The state stack
        std::vector<int> state;

        // Curent index into the state stack
        std::size_t level;
    };

private:

    // Persistant state
    PersistantState perState;

    // Persistant state pointer
    PersistantState * pState;

    // This states level
    std::size_t level;
};
