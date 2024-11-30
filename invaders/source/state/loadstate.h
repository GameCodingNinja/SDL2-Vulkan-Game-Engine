
/************************************************************************
*    FILE NAME:       loadstate.h
*
*    DESCRIPTION:     All this state does is unload/load and
*                     does a state change.
************************************************************************/

#pragma once

// Physical component dependency
#include "commonstate.h"

// Standard lib dependencies
#include <string>
#include <memory>

class CLoadState : public CCommonState
{
public:

    // Constructor
    CLoadState( const CStateMessage & stateMsg );

    // Destructor
    virtual ~CLoadState();
    
    // Do any pre-load init's
    void init() override;
    
    // Handle events
    void handleEvent( const SDL_Event & rEvent ) override;

    // Is the state done
    bool doStateChange() override;

    // Get the next state to load
    NStateDefs::EGameState getNextState() override;

private:

    // Load the assets
    void assetLoad();
    
private:
    
    // Exception text
    std::string m_errorTitle;
    std::string m_errorMsg;
};
