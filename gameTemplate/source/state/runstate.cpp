
/************************************************************************
*    FILE NAME:       runstate.cpp
*
*    DESCRIPTION:     CRunState Class State
************************************************************************/

// Physical component dependency
#include "runstate.h"

// Game lib dependencies
//#include <gui/menumanager.h>
#include <utilities/highresolutiontimer.h>
#include <objectdata/objectdata2d.h>
#include <objectdata/objectdatamanager.h>
#include <system/device.h>
//#include <physics/physicsworldmanager2d.h>
//#include <physics/physicsworld2d.h>
//#include <physics/physicscomponent2d.h>
#include <managers/cameramanager.h>
//#include <strategy/basicstagestrategy.h>
//#include <strategy/basicspritestrategy.h>
//#include <strategy/strategymanager.h>

// Standard lib dependencies
#include <vector>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CRunState::CRunState() :
    CCommonState( NGameDefs::EGS_RUN, NGameDefs::EGS_GAME_LOAD )//,
        //m_rPhysicsWorld( CPhysicsWorldManager2D::Instance().getWorld( "(game)" ) )
{
}


/************************************************************************
*    DESC:  Do any pre-game loop init's
************************************************************************/
void CRunState::init()
{
    // Unblock the menu messaging and activate needed trees
    //CMenuMgr::Instance().allow();
    //CMenuMgr::Instance().activateTree("pause_tree");

    // Prepare the script to fade in the screen
    //m_scriptComponent.prepare( "(menu)", "Screen_FadeIn" );

    // Reset the elapsed time before entering game loop
    CHighResTimer::Instance().calcElapsedTime();
}


/************************************************************************
*    DESC:  Handle events
************************************************************************/
void CRunState::handleEvent( const SDL_Event & rEvent )
{
    CCommonState::handleEvent( rEvent );

    // Check for the "change state" message
    /*if( rEvent.type == NMenu::EGE_MENU_GAME_STATE_CHANGE )
    {
        // Prepare the script to fade in the screen. The script will send the end message
        if( rEvent.user.code == NMenu::ETC_BEGIN )
            m_scriptComponent.prepare( "(menu)", "Screen_FadeOut" );
    }*/
}


/***************************************************************************
*    DESC:  Handle the physics
****************************************************************************/
void CRunState::physics()
{
    /*if( !CMenuMgr::Instance().isActive() )
    {
        m_rPhysicsWorld.fixedTimeStep();
    }*/
}


/***************************************************************************
*    DESC:  Update objects that require them
****************************************************************************/
void CRunState::update()
{
    CCommonState::update();

    //m_scriptComponent.update();

    /*if( !CMenuMgr::Instance().isActive() )
        CStrategyMgr::Instance().update();*/
}


/***************************************************************************
*    DESC:  Transform the game objects
****************************************************************************/
void CRunState::transform()
{
    CCommonState::transform();

    /*if( !CMenuMgr::Instance().isActive() )
        CStrategyMgr::Instance().transform();*/
}


/***************************************************************************
*    DESC:  Do the 2D rendering
****************************************************************************/
void CRunState::preRender()
{
    CCommonState::preRender();

    //CStrategyMgr::Instance().render( CCameraMgr::Instance().getDefaultProjMatrix() );
}


/***************************************************************************
*    DESC: Functions for loading/unloading the assets for this state
****************************************************************************/
namespace NRunState
{
    /***************************************************************************
    *    DESC:  Namespace function for loading the assets for this state
    *           NOTE: Only call when the class is not allocated
    ****************************************************************************/
    void ObjectDataLoad()
    {
        //CObjectDataMgr::Instance().loadGroup2D( "(run)");
    }

    void CriticalLoad()
    {
        // Create the group's VBO, IBO, textures, etc
        //CObjectDataMgr::Instance().createFromData2D( "(run)" );
    }

    void Load()
    {
	// All physics entities are destroyed and all heap memory is released.
        //CPhysicsWorldManager2D::Instance().createWorld( "(game)" );
        //CStrategyMgr::Instance().addStrategy( "(stage1)", new CBasicStageStrategy );
        //CStrategyMgr::Instance().addStrategy( "(sprite)", new CBasicSpriteStrategy );
    }

    void CriticalInit()
    {
        /*const char* shapes[] = {"triangle_blue", "triangle_green", "circle_blue", "circle_green", "circle_red", "square_red"};

        for( int i = 0; i < 24; ++i )
            CStrategyMgr::Instance().create( "(sprite)", shapes[i % 6] );*/
    }


    /***************************************************************************
    *    DESC:  Namespace function for unloading the assets for this state
    *           NOTE: Only call when the class is not allocated
    ****************************************************************************/
    void CriticalUnload()
    {
        //CStrategyMgr::Instance().cleanUp();
        //CObjectDataMgr::Instance().freeGroup2D( "(run)" );
    }

    void Unload()
    {
        //CStrategyMgr::Instance().clear();
        //CPhysicsWorldManager2D::Instance().destroyWorld( "(game)" );
    }
}
