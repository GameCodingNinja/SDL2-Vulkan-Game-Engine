
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
//#include <2d/sprite2d.h>
//#include <3d/sprite3d.h>
#include <slot/slotgame.h>
#include <script/scriptcomponent.h>

// Standard lib dependencies
#include <tuple>

// Forward declaration(s)
class CUIMeter;

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
    
    // Handle any misc processing before the real work is started
    void miscProcess() override;
    
    // Update objects that require them
    void update() override;

    // Transform the game objects
    void transform() override;

    // 2D/3D Render of game content
    //void preRender() override;
    
private:

    // The script component
    CScriptComponent m_scriptComponent;
    
    // title screen background
    //CSprite2D m_background;
    
    //CSprite2D m_spriteSheetTest;
    
    //CSprite3D m_cube;
};


/***************************************************************************
*    desc: Functions for loading/unloading the assets for this state
****************************************************************************/
namespace NTitleScreenState
{
    void Load();
    void Unload();
}


#endif  // __title_screen_state_h__


