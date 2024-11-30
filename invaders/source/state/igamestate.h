
/************************************************************************
*    FILE NAME:       igamestate.h
*
*    DESCRIPTION:     gamestate interface Class
************************************************************************/

#pragma once

// Game dependencies
#include "statemessage.h"

// Game lib dependencies
#include <common/defs.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Forward declaration(s)
union SDL_Event;

class iGameState : public boost::noncopyable
{
public:

    // Constructor
    iGameState( const NStateDefs::EGameState gameState, const NStateDefs::EGameState nextState )  :
        m_gameState(gameState),
        m_nextState(nextState)
    {};

    // Constructor
    iGameState( const NStateDefs::EGameState gameState, const CStateMessage & stateMsg ) :
        m_gameState(gameState),
        m_nextState(NStateDefs::EGS_GAME_LOAD),
        m_stateMessage(stateMsg)
    {};

    // Destructor
    virtual ~iGameState(){};
    
    // Do any pre-game loop init's
    virtual void init(){};

    // Handle events
    virtual void handleEvent( const SDL_Event & rEvent ){};

    // Act upon what the user is doing
    virtual void getUserInput( bool hasFocus ){};

    // Handle the physics
    virtual void physics(){};

    // Update objects that require them
    virtual void update(){};

    // Transform the game objects
    virtual void transform(){};
    
    // Record the command buffer vector in the device
    // for all the sprite objects that are to be rendered
    virtual void recordCommandBuffer( uint32_t cmdBufIndex ){};

    // Is the state done
    virtual bool doStateChange()
    { return m_changeState; };

    // Get the correct game state
    NStateDefs::EGameState getState()
    { return m_gameState; }

    // Get the next game state
    virtual NStateDefs::EGameState getNextState()
    { return m_nextState; }

    // Get the message
    const CStateMessage & getStateMessage()
    { return m_stateMessage; }

protected:

    // This objects current game state value
    const NStateDefs::EGameState m_gameState;

    // This objects next game state value
    const NStateDefs::EGameState m_nextState;
    
    // Message to send to next state
    CStateMessage m_stateMessage;
    
    // Flag to indicate state change
    bool m_changeState = false;

};
