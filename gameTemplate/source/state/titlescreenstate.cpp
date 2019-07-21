
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
#include <strategy/actorstrategy.h>
#include <script/scriptmanager.h>
#include <gui/menumanager.h>
#include <node/inode.h>
#include <sprite/sprite.h>

// SDL lib dependencies
#include <SDL.h>

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
    // Wait for all rendering to be finished
    CDevice::Instance().waitForIdle();
    
    CStrategyMgr::Instance().deleteStrategy( "(title)" );
    CStrategyMgr::Instance().deleteStrategy( "(cube)" );
    CDevice::Instance().deleteCommandPoolGroup( "(title)" );
    CObjectDataMgr::Instance().freeGroup( "(title)" );
    CObjectDataMgr::Instance().freeGroup( "(cube)" );
}


/************************************************************************
*    DESC:  Do any pre-game loop init's
************************************************************************/
void CTitleScreenState::init()
{
    // Activate the needed tree(s)
    CMenuMgr::Instance().activateTree( "title_screen_tree" );
    
    // Enable the strategy for rendering
    // Command buffers can only be used in the thread they are created
    auto titleCmdBufVec = CDevice::Instance().createSecondaryCommandBuffers( "(title)" );
    CStrategyMgr::Instance().activateStrategy( "(title)" )->setCommandBuffers( titleCmdBufVec );
    
    // Command buffers can only be used in the thread they are created
    auto cubeCmdBufVec = CDevice::Instance().createSecondaryCommandBuffers( "(title)" );
    CStrategyMgr::Instance().activateStrategy( "(cube)" )->setCommandBuffers( cubeCmdBufVec );
    
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
    if( rEvent.type == NMenuDefs::EME_MENU_GAME_STATE_CHANGE )
    {
        // Prepare the script to fade in the screen
        if( rEvent.user.code == NMenuDefs::ETC_BEGIN )
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
    
    // Add the strategies
    auto titleStrategy = CStrategyMgr::Instance().addStrategy( "(title)", new CActorStrategy() );
    auto cubeStrategy = CStrategyMgr::Instance().addStrategy( "(cube)", new CActorStrategy() );
    cubeStrategy->setCamera( "cubeCamera" );
    
    // Add the actors
    titleStrategy->create( "background" );
    cubeStrategy->create( "cube" )->getSprite()->prepare( "rotate" );
}
