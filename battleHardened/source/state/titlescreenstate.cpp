
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
#include <managers/cameramanager.h>
#include <gui/menumanager.h>
#include <common/camera.h>

// SDL lib dependencies
#include <SDL.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CTitleScreenState::CTitleScreenState() :
    CCommonState( NGameDefs::EGS_TITLE_SCREEN, NGameDefs::EGS_GAME_LOAD ),
        m_background( CObjectDataMgr::Instance().getData2D( "(title_screen)", "background" ) )
{
}


/************************************************************************
*    DESC:  Do any pre-game loop init's
************************************************************************/
void CTitleScreenState::init()
{
    CCommonState::init();

    // Unblock the menu messaging and activate needed trees
    CMenuMgr::Instance().allow();
    CMenuMgr::Instance().activateTree( "title_screen_tree");

    // Prepare the script to fade in the screen
    m_scriptComponent.prepare( "(menu)", "Screen_FadeIn" );

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
    if( rEvent.type == NMenu::EGE_MENU_GAME_STATE_CHANGE )
    {
        // Prepare the script to fade in the screen. The script will send the end message
        if( rEvent.user.code == NMenu::ETC_BEGIN )
            m_scriptComponent.prepare( "(menu)", "Screen_FadeOut" );
    }
}


/***************************************************************************
*    DESC:  Is the state done
****************************************************************************/
/*bool CTitleScreenState::DoStateChange()
{
    if( m_changeState )
    {
        m_stateMessage.Clear();
        m_stateMessage.m_nextState = EGS_Level_1;
        m_stateMessage.m_lastState = EGS_TITLE_SCREEN;

        // Unload the object data
        m_stateMessage.m_groupUnload.push_back("(title_screen)");

        // Load the object data
        m_stateMessage.m_groupLoad.push_back("(stage1)");
        m_stateMessage.m_groupLoad.push_back("(actor)");

        // Load script group
        m_stateMessage.m_scriptLoad.push_back("(actor)");

        // Stage to load
        m_stateMessage.m_stageDataLoad = "(stage1)";

        // Actor data load
        m_stateMessage.m_actorDataLoad = "(actor)";

        return true;
    }

    return false;

}*/


/***************************************************************************
*    DESC:  Update objects that require them
****************************************************************************/
void CTitleScreenState::update()
{
    CCommonState::update();

    m_scriptComponent.update();
}


/***************************************************************************
*    DESC:  Transform the game objects
****************************************************************************/
void CTitleScreenState::transform()
{
    CCommonState::transform();

    m_background.transform();
}


/***************************************************************************
*    DESC:  2D/3D Render of game content
****************************************************************************/
void CTitleScreenState::preRender()
{
    CCommonState::preRender();

    m_background.render( CCameraMgr::Instance().getDefaultProjMatrix()  );
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
        CObjectDataMgr::Instance().loadGroup2D( "(title_screen)", CObjectDataMgr::DONT_CREATE_FROM_DATA );
    }

    void CriticalLoad()
    {
        // Create the group's VBO, IBO, textures, etc
        CObjectDataMgr::Instance().createFromData2D( "(title_screen)" );
    }

    void Load()
    {

    }   // ThreadLoad

    void CriticalInit()
    {

    }


    /***************************************************************************
    *    DESC:  Namespace function for unloading the assets for this state
    *           NOTE: Only call when the class is not allocated
    ****************************************************************************/
    void CriticalUnload()
    {
        CObjectDataMgr::Instance().freeGroup2D( "(title_screen)" );
    }

    void Unload()
    {
    }

}
