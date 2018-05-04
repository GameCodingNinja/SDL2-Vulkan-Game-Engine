
/************************************************************************
*    FILE NAME:       level1state.cpp
*
*    DESCRIPTION:     CState1State Class State
************************************************************************/

// Physical component dependency
#include "level1state.h"

// Game lib dependencies
#include <gui/menumanager.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/statcounter.h>
#include <objectdata/objectdata2d.h>
#include <objectdata/objectdatamanager.h>
#include <system/device.h>
#include <physics/physicsworld2d.h>
#include <managers/cameramanager.h>
#include <common/camera.h>
#include <script/scriptmanager.h>
#include <strategy/basicspritestrategy.h>
#include <strategy/loopstagestrategy.h>
#include <strategy/strategymanager.h>
#include <2d/actorsprite2d.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CLevel1State::CLevel1State() :
    CCommonState( NGameDefs::EGS_Level_1, NGameDefs::EGS_GAME_LOAD )
{
}


/************************************************************************
*    DESC:  Do any pre-game loop init's
************************************************************************/
void CLevel1State::init()
{
    CCommonState::init();

    // Unblock the menu messaging and activate needed trees
    CMenuMgr::Instance().allow();
    CMenuMgr::Instance().activateTree("pause_tree");

    // Reset the camera
    //CCamera::Instance().SetPos( CStageMgr::Instance().GetDefaultCameraPos().GetPos() );

    // Prepare the script to fade in the screen
    m_scriptComponent.prepare( "(menu)", "Screen_FadeIn" );

    // Create the actors
    CStrategyMgr::Instance().create("(actor)", "enemy_ship");
    m_pPlayerShip = CStrategyMgr::Instance().createSprite<CActorSprite2D>("(actor)", "player_ship");

    // Get pointer to the player ship
    //m_pPlayerShip = &CSpriteStrategyMgr::Instance().Get<CBasicSpriteStrategy2D>("(actor)").Get<CActorSprite2D>(id);

    // Reset the elapsed time before entering game loop
    CHighResTimer::Instance().calcElapsedTime();
}


/************************************************************************
*    DESC:  Handle events
************************************************************************/
void CLevel1State::handleEvent( const SDL_Event & rEvent )
{
    CCommonState::handleEvent( rEvent );

    if( !CMenuMgr::Instance().isActive() )
    {
        m_pPlayerShip->handleEvent( rEvent );
    }

    // Check for the "change state" message
    if( rEvent.type == NMenu::EGE_MENU_GAME_STATE_CHANGE )
    {
        // Prepare the script to fade in the screen. The script will send the end message
        if( rEvent.user.code == NMenu::ETC_BEGIN )
            m_scriptComponent.prepare( "(menu)", "Screen_FadeOut" );
    }

    // Check for the "change state" message
    /*if( rEvent.type == NUIControl::ECAT_GAME_STATE_CHANGE )
    {
        m_stateMessage.Clear();
        m_stateMessage.m_nextState = EGS_TITLE_SCREEN;
        m_stateMessage.m_lastState = EGS_Level_1;

        // Load the object data
        m_stateMessage.m_groupLoad.push_back("(title_screen)");

        // Unload the object data
        m_stateMessage.m_groupUnload.push_back("(actor)");
        m_stateMessage.m_groupUnload.push_back("(stage1)");

        // Unload scripts
        m_stateMessage.m_scriptUnload.push_back("(actor)");

        m_nextState = EGS_GAME_LOAD;

        m_changeState = true;
    }*/
}


/************************************************************************
*    DESC:  Handle events
************************************************************************/
void CLevel1State::miscProcess()
{
    CStrategyMgr::Instance().miscProcess();
}


/***************************************************************************
*    DESC:  Handle the physics
****************************************************************************/
void CLevel1State::physics()
{
    if( !CMenuMgr::Instance().isActive() )
    {

    }
}


/***************************************************************************
*    DESC:  Update objects that require them
****************************************************************************/
void CLevel1State::update()
{
    CCommonState::update();

    m_scriptComponent.update();

    if( !CMenuMgr::Instance().isActive() )
    {
        //CCamera::Instance().IncPos( CPoint<float>( -0.05f * CHighResTimer::Instance().GetElapsedTime(), 0.f ) );
        CStrategyMgr::Instance().update();
    }
}


/***************************************************************************
*    DESC:  Transform the game objects
****************************************************************************/
void CLevel1State::transform()
{
    CCommonState::transform();

    //CCamera::Instance().Transform();
    CStrategyMgr::Instance().transform();
}


/***************************************************************************
*    DESC:  Do the 2D rendering
****************************************************************************/
void CLevel1State::preRender()
{
    CCommonState::preRender();

    CStrategyMgr::Instance().render( CCameraMgr::Instance().getDefaultProjMatrix() );
}



/***************************************************************************
*    DESC: Functions for loading/unloading the assets for this state
****************************************************************************/
namespace NLevel_1
{
    /***************************************************************************
    *    DESC:  Namespace function for loading the assets for this state
    *           NOTE: Only call when the class is not allocated
    ****************************************************************************/
    void ObjectDataLoad()
    {
        CObjectDataMgr::Instance().loadGroup2D( "(stage1)", CObjectDataMgr::DONT_CREATE_FROM_DATA );
        CObjectDataMgr::Instance().loadGroup2D( "(actor)", CObjectDataMgr::DONT_CREATE_FROM_DATA );
    }

    void CriticalLoad()
    {
        // Create the group's VBO, IBO, textures, etc
        CObjectDataMgr::Instance().createFromData2D( "(actor)" );
        CObjectDataMgr::Instance().createFromData2D( "(stage1)" );
    }

    void Load()
    {
        CStrategyMgr::Instance().addStrategy( "(actor)", new CBasicSpriteStrategy );
        CStrategyMgr::Instance().addStrategy( "(stage1)", new CLoopStageStrategy );

        // Load state specific AngelScript functions
        CScriptMgr::Instance().loadGroup("(actor)");
    }

    void CriticalInit()
    {
    }


    /***************************************************************************
    *    DESC:  Namespace function for unloading the assets for this state
    *           NOTE: Only call when the class is not allocated
    ****************************************************************************/
    void CriticalUnload()
    {
        CObjectDataMgr::Instance().freeGroup2D( "(stage1)" );
        CObjectDataMgr::Instance().freeGroup2D( "(actor)" );
    }

    void Unload()
    {
        CStrategyMgr::Instance().clear();
    }

}
