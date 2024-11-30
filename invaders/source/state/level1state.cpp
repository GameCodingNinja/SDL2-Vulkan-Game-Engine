
/************************************************************************
*    FILE NAME:       level1state.cpp
*
*    DESCRIPTION:     CLevel1State Class State
************************************************************************/

// Physical component dependency
#include "level1state.h"

// Game lib dependencies
#include <gui/menumanager.h>
#include <utilities/highresolutiontimer.h>
#include <objectdata/objectdata2d.h>
#include <objectdata/objectdatamanager.h>
#include <system/device.h>
#include <script/scriptmanager.h>
#include <physics/physicsworldmanager2d.h>
#include <physics/physicsworld2d.h>
#include <sprite/sprite.h>
#include <strategy/strategy.h>
#include <strategy/strategymanager.h>
#include <strategy/strategyloader.h>
#include <node/inode.h>

// Standard lib dependencies
#include <vector>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CLevel1State::CLevel1State() :
    CCommonState( NStateDefs::EGS_LEVEL_1, NStateDefs::EGS_GAME_LOAD )
{
}

/************************************************************************
*    DESC:  destructor
************************************************************************/
CLevel1State::~CLevel1State()
{
    CStrategyMgr::Instance().deleteStrategyLst( {"_level_1_"} );
    CDevice::Instance().deleteCommandPoolGroup( "(level_1)" );
    CObjectDataMgr::Instance().freeGroup( "(level_1)" );
    CScriptMgr::Instance().freeGroup("(level_1)");
}

/************************************************************************
*    DESC:  Do any pre-game loop init's
************************************************************************/
void CLevel1State::init()
{
    // Unblock the menu messaging and activate needed trees
    CMenuMgr::Instance().activateTree("pause_tree");

    // Activaye the strategies
    //CStrategyMgr::Instance().activateStrategyLst( {"_stage_", "_level_1_"} );

    // Start the fade
    m_scriptComponent.prepare( "(state)", "State_FadeIn" );
    
    // Reset the elapsed time before entering game loop
    CHighResTimer::Instance().calcElapsedTime();
}

/************************************************************************
*    DESC:  Handle events
************************************************************************/
void CLevel1State::handleEvent( const SDL_Event & rEvent )
{
    CCommonState::handleEvent( rEvent );
    
    // Event sent from menu
    if( rEvent.type == NMenuEvent::GAME_STATE_CHANGE )
    {
        // Prepare the script to fade in the screen.
        if( rEvent.user.code == NTransCode::BEGIN )
            m_scriptComponent.prepare( "(state)", "State_FadeOut" );
    }
    // Event sent from script
    else if( rEvent.type == NStateDefs::ESE_FADE_IN_COMPLETE )
    {
        CMenuMgr::Instance().allow();
    }
    // Event sent from script
    else if( rEvent.type == NStateDefs::ESE_FADE_OUT_COMPLETE )
    {
        // Clear out all the trees
        CMenuMgr::Instance().clearActiveTrees();
            
        // Set the flag to change the state
        m_changeState = true;
    }
}

/***************************************************************************
*    DESC:  Static function for loading the assets for this state
*           NOTE: Only call when the class is not allocated
****************************************************************************/
void CLevel1State::load()
{
    // Load the level 1 script group
    CScriptMgr::Instance().loadGroup("(level_1)");

    // Load the level 1 art
    CObjectDataMgr::Instance().loadGroup( "(level_1)");

    // Load the Strategy
    CStrategyloader::Instance().loadGroup( "-level1-" );
}
