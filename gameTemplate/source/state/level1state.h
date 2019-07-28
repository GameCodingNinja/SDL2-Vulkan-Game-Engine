
/************************************************************************
*    FILE NAME:       level1state.h
*
*    DESCRIPTION:     CLevel1State Class State
************************************************************************/

#ifndef __level_1_state_h__
#define __level_1_state_h__

// Physical component dependency
#include "commonstate.h"

// Forward declaration(s)
class CPhysicsWorld2D;

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

    // Handle the physics
    void physics() override;
    
    // Static load function
    static void load();

private:
    
    // Physics world
    CPhysicsWorld2D & m_rPhysicsWorld;
};

#endif
