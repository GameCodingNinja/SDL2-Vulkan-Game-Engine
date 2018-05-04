
/************************************************************************
*    FILE NAME:       commonstate.h
*
*    DESCRIPTION:     CCommonState Class State
************************************************************************/

#ifndef __common_state_h__
#define __common_state_h__

// Physical component dependency
#include "igamestate.h"

class CCommonState : public iGameState
{
public:

    // Constructor
    CCommonState();

    // Destructor
    virtual ~CCommonState();

    // Handle events
    virtual void HandleEvent( const SDL_Event & rEvent );

    // Update objects that require them
    virtual void Update();

    // Transform the game objects
    virtual void Transform();

    // Do the rendering
    virtual void PostRender();

};

#endif  // __common_state_h__


