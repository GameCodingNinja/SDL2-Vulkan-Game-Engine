
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
#include <node/inode.h>
#include <sprite/sprite.h>
#include <script/scriptmanager.h>
#include <gui/menumanager.h>
//#include <gui/uibutton.h>
//#include <gui/uimeter.h>

// SDL lib dependencies
#include <SDL.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CTitleScreenState::CTitleScreenState() :
    CCommonState( NGameDefs::EGS_TITLE_SCREEN, NGameDefs::EGS_GAME_LOAD ),
    m_pBackground(nullptr)
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CTitleScreenState::~CTitleScreenState()
{
    CStrategyMgr::Instance().clear();
    CDevice::Instance().deleteCommandPoolGroup( "(title_screen)" );
    CObjectDataMgr::Instance().freeGroup2D( "(title_screen)" );
    CScriptMgr::Instance().freeGroup("(title_screen)");
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
    
    // Create the group command buffers
    auto commandBufVec = CDevice::Instance().createSecondaryCommandBuffers( "(title_screen)" );

    // Add the actor strategy
    CStrategyMgr::Instance().addStrategy(
        "actorStrategy", new CActorStrategy( "data/objects/2d/spritestrategy/titlescreenNode.lst", commandBufVec ) );
    
    // Start the fade in
    m_pBackground = CStrategyMgr::Instance().create( "actorStrategy", "background" );
    m_pBackground->getSprite()->prepare( "fadeIn" );

    // Reset the elapsed time before entering game loop
    CHighResTimer::Instance().calcElapsedTime();
}


/************************************************************************
*    DESC:  Handle events
************************************************************************/
void CTitleScreenState::handleEvent( const SDL_Event & rEvent )
{
    CCommonState::handleEvent( rEvent );

    // Check for the "change state" message
    if( rEvent.type == NMenuDefs::EME_MENU_GAME_STATE_CHANGE )
    {
        // Prepare the script to fade in the screen. The script will send the end message
        if( rEvent.user.code == NMenuDefs::ETC_BEGIN )
            m_pBackground->getSprite()->prepare( "fadeOut" );
    }
    else if( rEvent.type == NGameDefs::EGE_FADE_IN_COMPLETE )
    {
        CMenuMgr::Instance().allow();
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
*    DESC:  Namespace function for loading the assets for this state
*           NOTE: Only call when the class is not allocated
****************************************************************************/
void CTitleScreenState::load()
{
    CObjectDataMgr::Instance().loadGroup2D( "(title_screen)" );
    CScriptMgr::Instance().loadGroup("(title_screen)");

    //CObjectDataMgr::Instance().LoadGroup2D( "(actor)", CObjectDataMgr::DONT_CREATE_FROM_DATA );
    //CObjectDataMgr::Instance().loadGroup3D( "(cube)" );
}
