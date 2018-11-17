
/************************************************************************
*    FILE NAME:       runstate.h
*
*    DESCRIPTION:     CRunState Class State
************************************************************************/

#ifndef __run_up_state_h__
#define __run_up_state_h__

// Physical component dependency
#include "commonstate.h"

// Forward declaration(s)
class CPhysicsWorld2D;

class CRunState : public CCommonState
{
public:

    // Constructor
    CRunState();

    // Destructor
    virtual ~CRunState();
    
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
