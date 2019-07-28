
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
#include <strategy/stagestrategy.h>
#include <strategy/actorstrategy.h>
#include <strategy/strategymanager.h>
#include <strategy/strategyloader.h>

// Standard lib dependencies
#include <vector>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CLevel1State::CLevel1State() :
    CCommonState( NStateDefs::EGS_LEVEL_1, NStateDefs::EGS_GAME_LOAD ),
        m_rPhysicsWorld( CPhysicsWorldManager2D::Instance().getWorld( "(game)" ) )
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CLevel1State::~CLevel1State()
{
    // Wait for all rendering to be finished
    CDevice::Instance().waitForIdle();
    
    CStrategyMgr::Instance().deleteStrategyLst( {"_level_1_", "_stage_"} );
    CDevice::Instance().deleteCommandPoolGroup( "(level_1)" );
    CObjectDataMgr::Instance().freeGroup( "(level_1)" );
    CPhysicsWorldManager2D::Instance().destroyWorld( "(game)" );
}


/************************************************************************
*    DESC:  Do any pre-game loop init's
************************************************************************/
void CLevel1State::init()
{
    // Unblock the menu messaging and activate needed trees
    CMenuMgr::Instance().activateTree("pause_tree");

    // Activaye the strategies
    CStrategyMgr::Instance().activateStrategyLst( {"_stage_", "_level_1_"} );

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
    if( rEvent.type == NMenuDefs::EME_MENU_GAME_STATE_CHANGE )
    {
        // Prepare the script to fade in the screen.
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
*    DESC:  Handle the physics
****************************************************************************/
void CLevel1State::physics()
{
    if( !CMenuMgr::Instance().isActive() )
    {
        m_rPhysicsWorld.fixedTimeStep();
    }
}


/***************************************************************************
*    DESC:  Static function for loading the assets for this state
*           NOTE: Only call when the class is not allocated
****************************************************************************/
void CLevel1State::load()
{
    CObjectDataMgr::Instance().loadGroup( "(level_1)");
    
    // Create the physics world
    CPhysicsWorldManager2D::Instance().createWorld( "(game)" );

    // Load the Strategy
    NStrategyloader::load( "data/objects/strategy/level_1/strategy.loader" );
}
