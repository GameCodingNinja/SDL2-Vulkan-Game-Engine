
/************************************************************************
*    FILE NAME:       titlescreenstate.h
*
*    DESCRIPTION:     CRunState Class State
************************************************************************/

#pragma once

// Physical component dependency
#include "commonstate.h"

// Forward Declarations
class iStrategy;

class CTitleScreenState : public CCommonState
{
public:

    // Constructor
    CTitleScreenState();

    // Destructor
    virtual ~CTitleScreenState();
    
    // Do any pre-game loop init's
    void init() override;
    
    // Handle events
    void handleEvent( const SDL_Event & rEvent ) override;
    
    // Static load function
    static void load();
};
