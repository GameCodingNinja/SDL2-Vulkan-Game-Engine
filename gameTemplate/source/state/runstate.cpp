
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
//#include <physics/physicsworldmanager2d.h>
//#include <physics/physicsworld2d.h>
//#include <physics/physicscomponent2d.h>
#include <managers/cameramanager.h>
#include <strategy/basicstagestrategy.h>
#include <strategy/actorstrategy.h>
#include <strategy/strategymanager.h>

// Standard lib dependencies
#include <vector>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CRunState::CRunState() :
    CCommonState( NGameDefs::EGS_RUN, NGameDefs::EGS_GAME_LOAD )
        //m_rPhysicsWorld( CPhysicsWorldManager2D::Instance().getWorld( "(game)" ) )
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CRunState::~CRunState()
{
    CStrategyMgr::Instance().deleteStrategy( "(run)" );
    CStrategyMgr::Instance().deleteStrategy( "(stage)" );
    CDevice::Instance().deleteCommandPoolGroup( "(run)" );
    CObjectDataMgr::Instance().freeGroup2D( "(run)" );
}


/************************************************************************
*    DESC:  Do any pre-game loop init's
************************************************************************/
void CRunState::init()
{
    // Unblock the menu messaging and activate needed trees
    CMenuMgr::Instance().activateTree("pause_tree");

    // Enable the strategy for rendering
    CStrategyMgr::Instance().getStrategy( "(stage)" )->enable();
    CStrategyMgr::Instance().getStrategy( "(run)" )->enable();

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
/*void CRunState::physics()
{
    if( !CMenuMgr::Instance().isActive() )
    {
        m_rPhysicsWorld.fixedTimeStep();
    }
}*/


/***************************************************************************
*    DESC:  Transform the game objects
****************************************************************************/
/*void CRunState::transform()
{
    CCommonState::transform();

    if( !CMenuMgr::Instance().isActive() )
        CStrategyMgr::Instance().transform();
}*/


/***************************************************************************
*    DESC:  Do the 2D rendering
****************************************************************************/
/*void CRunState::preRender()
{
    CCommonState::preRender();

    //CStrategyMgr::Instance().render( CCameraMgr::Instance().getDefaultProjMatrix() );
}*/


/***************************************************************************
*    DESC:  Static function for loading the assets for this state
*           NOTE: Only call when the class is not allocated
****************************************************************************/
void CRunState::load()
{
    CObjectDataMgr::Instance().loadGroup2D( "(run)");
    
    // Add a stage strategy
    auto stageCmdBufVec = CDevice::Instance().createSecondaryCommandBuffers( "(run)" );
    CStrategyMgr::Instance().addStrategy( "(stage)", new CBasicStageStrategy( stageCmdBufVec ) );
    
    // Add an actor strategy
    auto runCmdBufVec = CDevice::Instance().createSecondaryCommandBuffers( "(run)" );
    CStrategyMgr::Instance().addStrategy( "(run)", new CActorStrategy( runCmdBufVec ) );
    
    // All physics entities are destroyed and all heap memory is released.
    //CPhysicsWorldManager2D::Instance().createWorld( "(game)" );
    //CStrategyMgr::Instance().addStrategy( "(stage1)", new CBasicStageStrategy );
    //CStrategyMgr::Instance().addStrategy( "(sprite)", new CBasicSpriteStrategy );
}
