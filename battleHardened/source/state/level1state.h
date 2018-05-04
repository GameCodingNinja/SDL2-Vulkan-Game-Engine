
/************************************************************************
*    FILE NAME:       level1state.h
*
*    DESCRIPTION:     CState1State Class State
************************************************************************/

#ifndef __stage_1_state_h__
#define __stage_1_state_h__

// Physical component dependency
#include "commonstate.h"

// Game lib dependencies
#include <2d/sprite2d.h>

// Forward declaration(s)
class CActorSprite2D;

class CLevel1State : public CCommonState
{
public:

    // Constructor
    CLevel1State();

    // Destructor
    virtual ~CLevel1State(){};
    
    // Do any pre-game loop init's
    void init() override;
    
    // Handle events
    void handleEvent( const SDL_Event & rEvent ) override;
    
    // Handle any misc processing before the real work is started
    void miscProcess() override;

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
    
    // Player ship
    CActorSprite2D * m_pPlayerShip;
};


namespace NLevel_1
{
    void ObjectDataLoad();
    void CriticalLoad();
    void CriticalUnload();
    void Load();
    void Unload();
    void CriticalInit();
    
}   // NLevel_1

#endif  // __stage_1_state_h__


