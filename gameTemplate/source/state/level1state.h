
/************************************************************************
*    FILE NAME:       level1state.h
*
*    DESCRIPTION:     CLevel1State Class State
************************************************************************/

#pragma once

// Physical component dependency
#include "commonstate.h"

// Box2D lib dependencies
#include <Box2D/Dynamics/b2WorldCallbacks.h>

// Forward declaration(s)
class CPhysicsWorld2D;

class CLevel1State : public CCommonState, b2ContactListener//, b2DestructionListener
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

    // Called when two fixtures begin to touch
    void BeginContact(b2Contact* contact) override;
    
    // Called when two fixtures cease to touch
    void EndContact(b2Contact* contact) override;

    // Called when any fixture is about to be destroyed
    //void SayGoodbye(b2Fixture* fixture) override {}
    
    // Called when any joint is about to be destroyed
    //void SayGoodbye(b2Joint* joint) override {}

private:
    
    // Physics world
    CPhysicsWorld2D & m_rPhysicsWorld;

    // Sprite peg
    enum
    {
        SPRITE_PEG = -2
    };
};
