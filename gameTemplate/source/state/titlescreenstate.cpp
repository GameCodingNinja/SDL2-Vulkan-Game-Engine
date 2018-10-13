
/************************************************************************
*    FILE NAME:       titlescreenstate.cpp
*
*    DESCRIPTION:     Class template
************************************************************************/

// Physical component dependency
#include "titlescreenstate.h"

// Game lib dependencies
#include <objectdata/objectdatamanager.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/xmlpreloader.h>
#include <managers/cameramanager.h>
//#include <gui/menumanager.h>
//#include <gui/uibutton.h>
//#include <gui/uimeter.h>

// SDL lib dependencies
#include <SDL.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CTitleScreenState::CTitleScreenState() :
    CCommonState( NGameDefs::EGS_TITLE_SCREEN, NGameDefs::EGS_GAME_LOAD )
        //m_background( CObjectDataMgr::Instance().getData2D( "(title_screen)", "background" ) ),
        //m_spriteSheetTest( CObjectDataMgr::Instance().GetData2D( "(title_screen)", "spriteSheetTest2" ) ),
        //m_cube( CObjectDataMgr::Instance().getData3D( "(cube)", "cube" ) )
{
}


/************************************************************************
*    DESC:  Do any pre-game loop init's
************************************************************************/
void CTitleScreenState::init()
{
    // Unblock the menu messaging and activate needed trees
    /*CMenuMgr::Instance().allow();
    CMenuMgr::Instance().activateTree( "title_screen_tree" );

    m_cube.setScale( 3, 3, 3 );

    CCameraMgr::Instance().createPerspective( "cube" );
    CCameraMgr::Instance().setActiveCameraPos( 0, 0, 20 );
    CCameraMgr::Instance().setActiveCameraRot( 10, 0, 0 );

    // Prepare the script to fade in the screen
    m_scriptComponent.prepare( "(menu)", "Screen_FadeIn" );*/

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
    /*if( rEvent.type == NMenu::EGE_MENU_GAME_STATE_CHANGE )
    {
        // Prepare the script to fade in the screen. The script will send the end message
        if( rEvent.user.code == NMenu::ETC_BEGIN )
            m_scriptComponent.prepare( "(menu)", "Screen_FadeOut" );
    }*/
}


/***************************************************************************
*    DESC:  Handle any misc processing before the real work is started
****************************************************************************/
void CTitleScreenState::miscProcess()
{

}


/***************************************************************************
*    DESC:  Update objects that require them
****************************************************************************/
void CTitleScreenState::update()
{
    CCommonState::update();

    m_scriptComponent.update();

    //float rot = CHighResTimer::Instance().getElapsedTime() * 0.04;
    //m_cube.incRot( rot, rot, 0 );
}


/***************************************************************************
*    DESC:  Transform the game objects
****************************************************************************/
void CTitleScreenState::transform()
{
    CCommonState::transform();

    //m_background.transform();

    CCameraMgr::Instance().transform();

    //m_spriteSheetTest.Transform();

    //m_cube.transform();
}


/***************************************************************************
*    DESC:  2D/3D Render of game content
****************************************************************************/
void CTitleScreenState::preRender()
{
    //m_background.render( CCameraMgr::Instance().getDefaultProjMatrix() );

    //m_spriteSheetTest.Render( orthoMatrix );

    CCommonState::preRender();

    //auto & camera = CCameraMgr::Instance().getActiveCamera();

    //m_cube.render( camera.getFinalMatrix(), camera.getRotMatrix() );
}


/***************************************************************************
*    DESC: Functions for loading/unloading the assets for this state
****************************************************************************/
namespace NTitleScreenState
{
    /***************************************************************************
    *    DESC:  Namespace function for loading the assets for this state
    *           NOTE: Only call when the class is not allocated
    ****************************************************************************/
    void ObjectDataLoad()
    {
        //CObjectDataMgr::Instance().loadGroup2D( "(title_screen)" );

        //CObjectDataMgr::Instance().LoadGroup2D( "(actor)", CObjectDataMgr::DONT_CREATE_FROM_DATA );
        //CObjectDataMgr::Instance().loadGroup3D( "(cube)" );
    }

    void CriticalLoad()
    {
        // Create the group's VBO, IBO, textures, etc
        //CObjectDataMgr::Instance().createFromData2D( "(title_screen)" );
        //CObjectDataMgr::Instance().createFromData3D( "(cube)" );
    }

    void Load()
    {

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
        //CObjectDataMgr::Instance().freeGroup2D( "(title_screen)" );
        //CObjectDataMgr::Instance().freeGroup3D( "(cube)" );
    }

    void Unload()
    {

    }
}
