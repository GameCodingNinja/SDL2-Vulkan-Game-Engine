
/************************************************************************
*    FILE NAME:       titlescreenstate.cpp
*
*    DESCRIPTION:     Class template
************************************************************************/

// Physical component dependency
#include "titlescreenstate.h"

// Game dependencies
#include "statedefs.h"

// Game lib dependencies
#include <objectdata/objectdatamanager.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/xmlpreloader.h>
#include <system/device.h>
#include <strategy/strategymanager.h>
#include <strategy/strategyloader.h>
#include <script/scriptmanager.h>
#include <gui/menumanager.h>
#include <node/inode.h>
#include <sprite/sprite.h>

// SDL lib dependencies
#include <SDL3/SDL.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CTitleScreenState::CTitleScreenState() :
    CCommonState( NStateDefs::EGS_TITLE_SCREEN, NStateDefs::EGS_GAME_LOAD )
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CTitleScreenState::~CTitleScreenState()
{
    CStrategyMgr::Instance().deleteStrategyLst( {"_title_", "_cube_"} );
    CDevice::Instance().deleteCommandPoolGroup( "(title)" );
    CObjectDataMgr::Instance().freeGroupLst( {"(title)", "(cube)"} );
}


/************************************************************************
*    DESC:  Do any pre-game loop init's
************************************************************************/
void CTitleScreenState::init()
{
    // Activate the needed tree(s)
    CMenuMgr::Instance().activateTree( "title_screen_tree" );

    // Activaye the strategies
    CStrategyMgr::Instance().activateStrategyLst( {"_title_", "_cube_"} );
    
    // Start the fade in
    m_scriptComponent.prepare( "(state)", "State_FadeIn" );

    // Reset the elapsed time before entering game loop
    CHighResTimer::Instance().calcElapsedTime();
}


/************************************************************************
*    DESC:  Handle events
************************************************************************/
void CTitleScreenState::handleEvent( const SDL_Event & rEvent )
{
    CCommonState::handleEvent( rEvent );

    // Event sent from menu
    if( rEvent.type == NMenuEvent::GAME_STATE_CHANGE )
    {
        // Prepare the script to fade in the screen
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
void CTitleScreenState::load()
{
    CObjectDataMgr::Instance().loadGroupLst( {"(title)", "(cube)"} );

    // Load the Strategies
    CStrategyloader::Instance().load( "data/objects/strategy/state/titlescreen.loader" );
}
