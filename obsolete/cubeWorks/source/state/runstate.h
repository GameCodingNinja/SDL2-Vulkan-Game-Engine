
/************************************************************************
*    FILE NAME:       runstate.h
*
*    DESCRIPTION:     CRunState Class State
************************************************************************/

#ifndef __run_up_state_h__
#define __run_up_state_h__

// Physical component dependency
#include "commonstate.h"

#include <2d/sprite2d.h>

// Forward declaration(s)
class CPhysicsWorld2d;

class CRunState : public CCommonState
{
public:

    // Constructor
    CRunState();

    // Destructor
    virtual ~CRunState(){};

    // Handle the physics
    void Physics();

    // Update objects that require them
    void Update();

    // Transform the game objects
    void Transform();

    // Do the rendering
    void PreRender();

    // Is the state done
    bool DoStateChange();

    // Handle events
    virtual void HandleEvent( const SDL_Event & rEvent );

private:

    // Flag to indicate state change
    bool m_changeState;
    
    CSprite2D m_box;
    CSprite2D m_triangle;
    CSprite2D m_circle;
    CSprite2D m_floor;
    
    CPhysicsWorld2d & m_physicsWorld;
};

#endif  // __run_up_state_h__


