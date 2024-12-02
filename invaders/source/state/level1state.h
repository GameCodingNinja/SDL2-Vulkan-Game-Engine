
/************************************************************************
*    FILE NAME:       level1state.h
*
*    DESCRIPTION:     CLevel1State Class State
************************************************************************/

#pragma once

// Physical component dependency
#include "commonstate.h"

// Forward Declarations
class CStrategy;

class CLevel1State : public CCommonState
{
public:

    // Constructor
    CLevel1State();

    // Destructor
    virtual ~CLevel1State();
    
    // Do any pre-game loop init's
    void init() override;

    // Handle events
    void handleEvent( const SDL_Event & rEvent ) override;
    
    // Static load function
    static void load();

private:
    
};
