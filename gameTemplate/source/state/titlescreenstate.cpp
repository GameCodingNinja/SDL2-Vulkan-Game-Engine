
/************************************************************************
*    FILE NAME:       titlescreenstate.cpp
*
*    DESCRIPTION:     Class template
************************************************************************/

// Physical component dependency
#include "titlescreenstate.h"

// Game dependencies
#include "gamedefs.h"

// Game lib dependencies
#include <objectdata/objectdatamanager.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/xmlpreloader.h>
#include <managers/cameramanager.h>
#include <system/device.h>
#include <strategy/strategymanager.h>
#include <strategy/actorstrategy.h>
#include <script/scriptmanager.h>
#include <gui/menumanager.h>

// SDL lib dependencies
#include <SDL.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CTitleScreenState::CTitleScreenState() :
    CCommonState( NGameDefs::EGS_TITLE_SCREEN, NGameDefs::EGS_GAME_LOAD )
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CTitleScreenState::~CTitleScreenState()
{
    CStrategyMgr::Instance().deleteStrategy( "(title)" );
    CDevice::Instance().deleteCommandPoolGroup( "(title)" );
    CObjectDataMgr::Instance().freeGroup2D( "(title)" );
    //CObjectDataMgr::Instance().freeGroup3D( "(cube)" );
}


/************************************************************************
*    DESC:  Do any pre-game loop init's
************************************************************************/
void CTitleScreenState::init()
{
    // Unblock the menu messaging and activate needed trees
    CMenuMgr::Instance().activateTree( "title_screen_tree" );

    /*m_cube.setScale( 3, 3, 3 );

    CCameraMgr::Instance().createPerspective( "cube" );
    CCameraMgr::Instance().setActiveCameraPos( 0, 0, 20 );
    CCameraMgr::Instance().setActiveCameraRot( 10, 0, 0 );*/
    
    // Enable the strategy for rendering
    CStrategyMgr::Instance().getStrategy( "(title)" )->enable();
    
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
*    DESC:  Update objects that require them
****************************************************************************/
/*void CTitleScreenState::update()
{
    CCommonState::update();

    m_scriptComponent.update();

    //float rot = CHighResTimer::Instance().getElapsedTime() * 0.04;
    //m_cube.incRot( rot, rot, 0 );
}*/


/***************************************************************************
*    DESC:  Transform the game objects
****************************************************************************/
/*void CTitleScreenState::transform()
{
    CCommonState::transform();

    //m_background.transform();

    CCameraMgr::Instance().transform();

    //m_spriteSheetTest.Transform();

    //m_cube.transform();
}*/


/***************************************************************************
*    DESC:  2D/3D Render of game content
****************************************************************************/
/*void CTitleScreenState::preRender()
{
    //m_background.render( CCameraMgr::Instance().getDefaultProjMatrix() );

    //m_spriteSheetTest.Render( orthoMatrix );

    CCommonState::preRender();

    //auto & camera = CCameraMgr::Instance().getActiveCamera();

    //m_cube.render( camera.getFinalMatrix(), camera.getRotMatrix() );
}*/


/***************************************************************************
*    DESC:  Static function for loading the assets for this state
*           NOTE: Only call when the class is not allocated
****************************************************************************/
void CTitleScreenState::load()
{
    CObjectDataMgr::Instance().loadGroup2D( "(title)" );
    
    // Add the actor strategy
    auto commandBufVec = CDevice::Instance().createSecondaryCommandBuffers( "(title)" );
    CStrategyMgr::Instance().addStrategy( "(title)", new CActorStrategy( commandBufVec ) );
    
    // Add the background
    CStrategyMgr::Instance().create( "(title)", "background" );

    //CObjectDataMgr::Instance().LoadGroup2D( "(actor)", CObjectDataMgr::DONT_CREATE_FROM_DATA );
    //CObjectDataMgr::Instance().loadGroup3D( "(cube)" );
}
