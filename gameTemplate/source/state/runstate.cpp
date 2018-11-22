
/************************************************************************
*    FILE NAME:       runstate.cpp
*
*    DESCRIPTION:     CRunState Class State
************************************************************************/

// Physical component dependency
#include "runstate.h"

// Game lib dependencies
#include <gui/menumanager.h>
#include <utilities/highresolutiontimer.h>
#include <objectdata/objectdata2d.h>
#include <objectdata/objectdatamanager.h>
#include <system/device.h>
#include <script/scriptmanager.h>
#include <physics/physicsworldmanager2d.h>
#include <physics/physicsworld2d.h>
#include <strategy/basicstagestrategy.h>
#include <strategy/actorstrategy.h>
#include <strategy/strategymanager.h>

// Standard lib dependencies
#include <vector>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CRunState::CRunState() :
    CCommonState( NGameDefs::EGS_RUN, NGameDefs::EGS_GAME_LOAD ),
        m_rPhysicsWorld( CPhysicsWorldManager2D::Instance().getWorld( "(game)" ) )
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CRunState::~CRunState()
{
    // Wait for all rendering to be finished
    CDevice::Instance().waitForIdle();
    
    CStrategyMgr::Instance().deleteStrategy( "(run)" );
    CStrategyMgr::Instance().deleteStrategy( "(stage)" );
    CDevice::Instance().deleteCommandPoolGroup( "(run)" );
    CObjectDataMgr::Instance().freeGroup2D( "(run)" );
    CPhysicsWorldManager2D::Instance().destroyWorld( "(game)" );
}


/************************************************************************
*    DESC:  Do any pre-game loop init's
************************************************************************/
void CRunState::init()
{
    // Unblock the menu messaging and activate needed trees
    CMenuMgr::Instance().activateTree("pause_tree");

    // Enable the strategy for rendering
    // Command buffers can only be used in the thread they are created
    auto stageCmdBufVec = CDevice::Instance().createSecondaryCommandBuffers( "(run)" );
    CStrategyMgr::Instance().getStrategy( "(stage)" )->init( stageCmdBufVec, true );
    
    // Command buffers can only be used in the thread they are created
    auto runCmdBufVec = CDevice::Instance().createSecondaryCommandBuffers( "(run)" );
    CStrategyMgr::Instance().getStrategy( "(run)" )->init( runCmdBufVec, true );

    // Start the fade
    m_scriptComponent.prepare( "(state)", "State_FadeIn" );
    
    // Reset the elapsed time before entering game loop
    CHighResTimer::Instance().calcElapsedTime();
}


/************************************************************************
*    DESC:  Handle events
************************************************************************/
void CRunState::handleEvent( const SDL_Event & rEvent )
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
    else if( rEvent.type == NGameDefs::EGE_FADE_IN_COMPLETE )
    {
        CMenuMgr::Instance().allow();
    }
    // Event sent from script
    else if( rEvent.type == NGameDefs::EGE_FADE_OUT_COMPLETE )
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
void CRunState::physics()
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
void CRunState::load()
{
    CObjectDataMgr::Instance().loadGroup2D( "(run)");
    
    // Create the physics world
    CPhysicsWorldManager2D::Instance().createWorld( "(game)" );
    
    // Add the strategies
    CStrategyMgr::Instance().addStrategy( "(stage)", new CBasicStageStrategy() );
    auto strategy = CStrategyMgr::Instance().addStrategy( "(run)", new CActorStrategy() );
    
    // Add the sprites to the strategy
    const char* shapes[] = {"triangle_blue", "triangle_green", "circle_blue", "circle_green", "circle_red", "square_red"};
    for( int i = 0; i < 24; ++i )
        strategy->create( shapes[i % 6] );
}
