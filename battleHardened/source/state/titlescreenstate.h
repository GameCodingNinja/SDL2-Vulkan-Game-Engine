
/************************************************************************
*    FILE NAME:       titlescreenstate.h
*
*    DESCRIPTION:     CRunState Class State
************************************************************************/

#ifndef __title_screen_state_h__
#define __title_screen_state_h__

// Physical component dependency
#include "commonstate.h"

// Game lib dependencies
#include <2d/sprite2d.h>
#include <script/scriptcomponent.h>

class CTitleScreenState : public CCommonState
{
public:

    // Constructor
    CTitleScreenState();

    // Destructor
    virtual ~CTitleScreenState(){};
    
    // Do any pre-game loop init's
    void init() override;
    
    // Handle events
    void handleEvent( const SDL_Event & rEvent ) override;
    
    // Update objects that require them
    void update() override;

    // Transform the game objects
    void transform() override;

    // 2D/3D Render of game content
    void preRender() override;

private:
    
    // The script conponent
    CScriptComponent m_scriptComponent;

    // title screen background
    CSprite2D m_background;
};


/***************************************************************************
*    desc: Functions for loading/unloading the assets for this state
****************************************************************************/
namespace NTitleScreenState
{
    void ObjectDataLoad();
    void CriticalLoad();
    void CriticalUnload();
    void Load();
    void Unload();
    void CriticalInit();
    
}   // NTitleScreenState


#endif  // __title_screen_state_h__


