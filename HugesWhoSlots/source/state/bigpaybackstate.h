
/************************************************************************
*    FILE NAME:       bigpaybackstate.h
*
*    DESCRIPTION:     The Big Pay Back game state
************************************************************************/

#ifndef __big_pay_back_state_h__
#define __big_pay_back_state_h__

// Physical component dependency
#include "commonstate.h"

// Game dependencies
#include "../game/frontpanel.h"

// Game lib dependencies
#include <2d/sprite2d.h>
#include <3d/sprite3d.h>
#include <slot/slotgame.h>
#include <slot/basegamemusic.h>
#include <script/scriptcomponent.h>

// Standard lib dependencies
#include <string>
#include <mutex>
#include <tuple>

class CBigPayBackState : public CCommonState
{
public:

    // Constructor
    CBigPayBackState( const std::string & group );

    // Destructor
    virtual ~CBigPayBackState(){};
    
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
    void preRender() override;
    
    // Do we allow lobby music?
    void allowMusic( bool allow ) override;
    
    // Do we allow the stop sounds?
    void allowStopSounds( bool allow ) override;
    
private:
    
    void totalBetCallBack(CUIControl *);
    
private:
    
    // Group name
    const std::string m_stateGroup;
    
    // The script conponent
    CScriptComponent m_scriptComponent;

    // Slot game class
    CSlotGame m_slotGame;
    
    // title screen background
    CSprite2D m_background;

    CSprite2D m_pig;
    
    // Front panel
    CFrontPanel m_frontPanel;
    
    // Base game music
    CBaseGameMusic m_baseGameMusic;

};


/***************************************************************************
*    desc: Functions for loading/unloading the assets for this state
****************************************************************************/
namespace NBigPayBack
{
    static std::tuple<std::string, std::string> reelGrpCfg("data/objects/2d/slot/bigPayBack/reelgroup.cfg", "reelgroup");
    static std::tuple<std::string, std::string> spinProfileCfg("data/objects/2d/slot/bigPayBack/spinProfile.cfg", "spinProfile");
    
    void ObjectDataLoad();
    void CriticalLoad();
    void CriticalUnload();
    void Load();
    void Unload();
    void CriticalInit();
    
}   // NBigPayBack

#endif  // __big_pay_back_state_h__


