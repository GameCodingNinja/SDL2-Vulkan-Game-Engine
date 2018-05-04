
/************************************************************************
*    FILE NAME:       runstate.h
*
*    DESCRIPTION:     CRunState Class State
************************************************************************/

#ifndef __run_up_state_h__
#define __run_up_state_h__

// Physical component dependency
#include "commonstate.h"

// Game lib dependencies
#include <2d/sprite2d.h>

// Forward declaration(s)
class CPhysicsWorld2D;

class CRunState : public CCommonState
{
public:

    // Constructor
    CRunState();

    // Destructor
    virtual ~CRunState(){};
    
    // Do any pre-game loop init's
    void init() override;

    // Handle events
    void handleEvent( const SDL_Event & rEvent ) override;

    // Handle the physics
    void physics() override;

    // Update objects that require them
    void update() override;

    // Transform the game objects
    void transform() override;

    // Do the rendering
    void preRender() override;

private:
    
    // The script conponent
    CScriptComponent m_scriptComponent;
    
    // Physics world
    CPhysicsWorld2D & m_rPhysicsWorld;
};


namespace NRunState
{
    void ObjectDataLoad();
    void CriticalLoad();
    void CriticalUnload();
    void Load();
    void Unload();
    void CriticalInit();
}


#endif  // __run_up_state_h__


