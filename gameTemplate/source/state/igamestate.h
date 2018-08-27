
/************************************************************************
*    FILE NAME:       igamestate.h
*
*    DESCRIPTION:     gamestate interface Class
************************************************************************/

#ifndef __i_game_state_h__
#define __i_game_state_h__

// Game dependencies
#include "statemessage.h"

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Forward declaration(s)
union SDL_Event;

class iGameState : public boost::noncopyable
{
public:

    // Constructor
    iGameState( const NGameDefs::EGameState gameState, const NGameDefs::EGameState nextState )  :
        m_gameState(gameState),
        m_nextState(nextState)
    {};

    // Constructor
    iGameState( const NGameDefs::EGameState gameState, const CStateMessage & stateMsg ) :
        m_gameState(gameState),
        m_nextState(NGameDefs::EGS_GAME_LOAD),
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
    
    // Handle any misc processing before the real work is started
    virtual void miscProcess(){};

    // Handle the physics
    virtual void physics(){};

    // Update objects that require them
    virtual void update(){};

    // Transform the game objects
    virtual void transform(){};

    // 2D/3D Render of game content
    virtual void render(){};
    virtual void preRender(){};
    virtual void postRender(){};

    // Is the state done
    virtual bool doStateChange()
    { return m_changeState; };

    // Get the correct game state
    NGameDefs::EGameState getState()
    { return m_gameState; }

    // Get the next game state
    virtual NGameDefs::EGameState getNextState()
    { return m_nextState; }

    // Get the message
    const CStateMessage & getStateMessage()
    { return m_stateMessage; }

protected:

    // This objects current game state value
    const NGameDefs::EGameState m_gameState;

    // This objects next game state value
    const NGameDefs::EGameState m_nextState;
    
    // Message to send to next state
    CStateMessage m_stateMessage;
    
    // Flag to indicate state change
    bool m_changeState = false;

};

#endif  // __i_game_state_h__
