
/************************************************************************
*    FILE NAME:       commonstate.h
*
*    DESCRIPTION:     CCommonState Class State
************************************************************************/

#pragma once

// Physical component dependency
#include "igamestate.h"

// Game lib dependencies
#include <script/scriptcomponent.h>

// Standard lib dependencies
#include <string>
#include <map>
#include <vector>

class CCommonState : public iGameState
{
public:

    // Constructor
    CCommonState( const NStateDefs::EGameState, const NStateDefs::EGameState );
    CCommonState( const NStateDefs::EGameState gameState, const CStateMessage & stateMsg );

    // Destructor
    virtual ~CCommonState();

    // Handle events
    virtual void handleEvent( const SDL_Event & rEvent ) override;

    // Update objects that require them
    virtual void update() override;

    // Transform the game objects
    void transform() override;
    
    // Record the command buffer vector in the device
    // for all the sprite objects that are to be rendered
    void recordCommandBuffer( uint32_t cmdBufIndex ) override;
    
protected:
    
    // Get the load state
    NStateDefs::EGameState getLoadState( void * pVoid );
    
protected:

    // Script component for screen fades
    CScriptComponent m_scriptComponent;
    
private:
        
    // game state map
    std::map<std::string, NStateDefs::EGameState> m_gameStateMap;
};
