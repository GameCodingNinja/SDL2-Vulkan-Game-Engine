
/************************************************************************
*    FILE NAME:       game.cpp
*
*    DESCRIPTION:     CGame class
************************************************************************/

// Physical component dependency
#include "game.h"

// Game lib dependencies
#include <common/build_defs.h>
#include <system/device.h>
#include <utilities/settings.h>
#include <utilities/statcounter.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/genfunc.h>
#include <strategy/strategymanager.h>
#include <managers/actionmanager.h>
#include <gui/menumanager.h>
#include <script/scriptmanager.h>
#include <script/scriptcolor.h>
#include <script/scriptsound.h>
#include <script/scriptplaylist.h>
#include <script/scriptpoint.h>
#include <script/scriptsize.h>
#include <script/scriptglobals.h>
#include <script/scriptsprite.h>
#include <script/scriptsoundmanager.h>
#include <script/scriptcamera.h>
#include <script/scriptmenu.h>
#include <script/scriptobjectdatamanager.h>
#include <script/scriptstrategymanager.h>
#include <script/scriptistrategy.h>
#include <script/scriptactionmanager.h>
#include <script/scriptsettings.h>
#include <script/scripthighresolutiontimer.h>
#include <script/scriptmenumanager.h>
#include <script/scriptfontmanager.h>
#include <script/scriptscriptmanager.h>
#include <script/scriptuicontrol.h>
#include <script/scriptdevice.h>
#include <script/scriptvisual.h>

// AngelScript lib dependencies
#include <scriptstdstring/scriptstdstring.h>
#include <scriptarray/scriptarray.h>
#include <scriptdictionary/scriptdictionary.h>
#include <scriptmath/scriptmath.h>

// Boost lib dependencies
#include <boost/bind.hpp>
#include <boost/format.hpp>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CGame::CGame()
{
    if( NBDefs::IsDebugMode() )
        CStatCounter::Instance().connect( boost::bind(&CGame::statStringCallBack, this, _1) );
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CGame::~CGame()
{
    // Wait for all rendering to be finished
    //CDevice::Instance().waitForIdle();
    //CScriptMgr::Instance().clear();
    
    // Free the assets
    //CMenuMgr::Instance().clear();
    //CStrategyMgr::Instance().clear();
    
    // Destroy the window and Vulkan instance
    //CDevice::Instance().destroy();

    // Quit SDL subsystems
    SDL_Quit();
}


/***************************************************************************
*   DESC:  Create the game Window
 ****************************************************************************/
void CGame::create()
{
    CDevice::Instance().setRecordCommandBufferCallback(
        std::bind( &CGame::recordCommandBuffer, this, std::placeholders::_1) );
    
    // Show the window
    //CDevice::Instance().showWindow( true );

    // Setup the message filtering
    SDL_SetEventFilter(FilterEvents, 0);

    // Handle some events on startup
    pollEvents();

    // Game start init
    init();
}


/************************************************************************
*    DESC:  Init the game
************************************************************************/
void CGame::init()
{
    // Handle some events on startup
    pollEvents();

    // Load the script list table
    CScriptMgr::Instance().loadListTable( "data/scripts/scriptListTable.lst" );

    // Register the script items
    RegisterStdString( CScriptMgr::Instance().getEnginePtr() );
    RegisterScriptArray( CScriptMgr::Instance().getEnginePtr(), false );
    RegisterScriptDictionary( CScriptMgr::Instance().getEnginePtr() );
    RegisterScriptMath( CScriptMgr::Instance().getEnginePtr() );
    NScriptGlobals::Register();
    NScriptColor::Register();
    NScriptPoint::Register();
    NScriptSize::Register();
    NScriptActionManager::Register();
    NScriptCamera::Register();
    NScriptSprite::Register();
    NScriptSound::Register();
    NScriptPlayLst::Register();
    NScriptiStrategy::Register();
    NScriptSoundManager::Register();
    NScriptObjectDataManager::Register();
    NScriptStrategyManager::Register();
    NScriptSettings::Register();
    NScriptHighResolutionTimer::Register();
    NScriptUIControl::Register();
    NScriptMenu::Register();
    NScriptMenuManager::Register();
    NScriptFontManager::Register();
    NScriptScriptManager::Register();
    NScriptDevice::Register();
    NScriptVisual::Register();

    CScriptMgr::Instance().loadGroup("(main)");
    CScriptMgr::Instance().prepare("(main)", "main");

    CHighResTimer::Instance().calcElapsedTime();
}


/***************************************************************************
*   DESC:  Poll for game events
****************************************************************************/
void CGame::pollEvents()
{
    // Event handler
    SDL_Event msgEvent;

    CActionMgr::Instance().clearQueue();

    // Handle events on queue
    while( SDL_PollEvent( &msgEvent ) )
    {
        CActionMgr::Instance().queueEvent( msgEvent );
        
        handleEvent( msgEvent );

        // let the game handle the event
        // turns true on quit
        /*if( handleEvent( msgEvent ) )
        {
            // Hide the window to give the impression of a quick exit
            CDevice::Instance().showWindow( false );

            break;
        }*/
    }
}


/***************************************************************************
*   DESC:  Main game loop
****************************************************************************/
bool CGame::gameLoop()
{
    // Poll for game events
    pollEvents();

    // Get our elapsed time
    CHighResTimer::Instance().calcElapsedTime();

    // Main script update
    return CScriptMgr::Instance().update();
}


/***************************************************************************
*    decs:  Record the command buffer vector in the device
*           for all the sprite objects that are to be rendered
****************************************************************************/
void CGame::recordCommandBuffer( uint32_t cmdBufIndex )
{
    CStrategyMgr::Instance().recordCommandBuffer( cmdBufIndex );
}


/************************************************************************
*    DESC:  Callback for the state string
************************************************************************/
void CGame::statStringCallBack( const std::string & statStr )
{
    if( !CSettings::Instance().getFullScreen() )
        SDL_SetWindowTitle( CDevice::Instance().getWindow(), statStr.c_str() );
}


/************************************************************************
*    DESC:  Handle events
************************************************************************/
bool CGame::handleEvent( const SDL_Event & rEvent )
{
    //if( (rEvent.type == SDL_QUIT) || (rEvent.type == SDL_APP_TERMINATING) )
    //    return true;
    
    // Handle events for the menu manager
    CMenuMgr::Instance().handleEvent( rEvent );
    
    // Check for the "game change state" message
    /*if( rEvent.type == NMenuDefs::EME_MENU_GAME_STATE_CHANGE )
    {
        // Block all message processing in the menu manager
        if( rEvent.user.code == NMenuDefs::ETC_BEGIN )
            CMenuMgr::Instance().allow( false );

        // Clear out all the trees
        else if( rEvent.user.code == NMenuDefs::ETC_END )
            CMenuMgr::Instance().clearActiveTrees();
    }*/

    // Filter out these events. Can't do this through the normal event filter
    if( (rEvent.type >= SDL_JOYAXISMOTION) && (rEvent.type <= SDL_JOYBUTTONUP) )
        return false;

    else if( rEvent.type == SDL_CONTROLLERDEVICEADDED )
        CDevice::Instance().addGamepad( rEvent.cdevice.which );

    else if( rEvent.type == SDL_CONTROLLERDEVICEREMOVED )
        CDevice::Instance().removeGamepad( rEvent.cdevice.which );

    else if( rEvent.type == SDL_APP_LOWMEMORY )
        displayErrorMsg( "Low Memory Error", "The device is experiencing low memory. Try freeing up some apps." );

    // In a traditional game, want the pause menu to display when the game is sent to the background
    else if( (rEvent.type == SDL_APP_WILLENTERBACKGROUND) && !CMenuMgr::Instance().isMenuActive() )
        NGenFunc::DispatchEvent( NMenuDefs::EME_MENU_ESCAPE_ACTION );

    return false;
}


/***************************************************************************
*   DESC:  Display error massage
****************************************************************************/
void CGame::displayErrorMsg( const std::string & title, const std::string & msg )
{
    CDevice::Instance().displayErrorMsg( title, msg );
}


/***************************************************************************
*    decs:  Filter out events we don't want
****************************************************************************/
int FilterEvents( void * userdata, SDL_Event * pEvent )
{
    // Return 0 to indicate that the event should be removed from the event queue

    // Do our own deadzone filtering
    if( pEvent->type == SDL_CONTROLLERAXISMOTION )
    {
        // Analog stick max values -32768 to 32767
        const int deadZone = CSettings::Instance().getGamePadStickDeadZone() *
            defs_ANALOG_PERCENTAGE_CONVERTION;

        if( ((pEvent->caxis.axis >= SDL_CONTROLLER_AXIS_LEFTX) &&
            (pEvent->caxis.axis <= SDL_CONTROLLER_AXIS_RIGHTY)) &&
            (((pEvent->caxis.value >= 0) && (pEvent->caxis.value < deadZone)) ||
            ((pEvent->caxis.value <= 0) && (pEvent->caxis.value > -deadZone))) )
            return 0;
    }

    // Return 1 to indicate that the event should stay in the event queue
    return 1;
}
