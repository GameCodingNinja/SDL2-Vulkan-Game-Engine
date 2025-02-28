
/************************************************************************
*    FILE NAME:       game.cpp
*
*    DESCRIPTION:     CGame class
************************************************************************/

// Physical component dependency
#include "game.h"

// Game lib dependencies
#include <system/device.h>
#include <utilities/settings.h>
#include <utilities/statcounter.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/genfunc.h>
#include <strategy/strategymanager.h>
#include <managers/actionmanager.h>
#include <managers/cameramanager.h>
#include <gui/menumanager.h>
#include <script/scriptmanager.h>
#include <script/scriptcolor.h>
#include <script/scriptsound.h>
#include <script/scriptpoint.h>
#include <script/scriptsize.h>
#include <script/scriptglobals.h>
#include <script/scriptsprite.h>
#include <script/scriptcameramanager.h>
#include <script/scriptcamera.h>
#include <script/scriptmenu.h>
#include <script/scriptobjectdatamanager.h>
#include <script/scriptstrategy.h>
#include <script/scriptactionmanager.h>
#include <script/scriptsettings.h>
#include <script/scripthighresolutiontimer.h>
#include <script/scriptmenumanager.h>
#include <script/scriptfontmanager.h>
#include <script/scriptscriptmanager.h>
#include <script/scriptuicontrol.h>
#include <script/scriptdevice.h>
#include <script/scriptvisual.h>
#include <script/scriptphysics2d.h>
#include <script/scriptstatcounter.h>
#include <script/scriptbitmask.h>
#include <script/scriptevent.h>

// AngelScript lib dependencies
#include <scriptstdstring/scriptstdstring.h>
#include <scriptarray/scriptarray.h>
#include <scriptdictionary/scriptdictionary.h>
#include <scriptmath/scriptmath.h>
#include <autowrapper/aswrappedcall.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CGame::CGame()
{
    // Init the device. NOTE: This always needs to be first
    // This call loads the settings XML
    CDevice::Instance().init( std::bind( &CGame::recordCommandBuffer, this, std::placeholders::_1) );

    // Load the camera data early because many objects init the default camera in their constructor
    // The above call to CDevice::Instance().init loads the settings which the camera relies on
    CCameraMgr::Instance().load( "data/objects/camera.lst" );
    
    if( CSettings::Instance().isDebugMode() )
        CStatCounter::Instance().connect( std::bind(&CGame::statStringCallBack, this, std::placeholders::_1) );
}

/************************************************************************
*    DESC:  destructor
************************************************************************/
CGame::~CGame()
{
}

/***************************************************************************
*   DESC:  Create the game Window
 ****************************************************************************/
void CGame::create()
{
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

    // Register the script items
    RegisterStdString( CScriptMgr::Instance().getEnginePtr() );
    RegisterScriptArray( CScriptMgr::Instance().getEnginePtr(), true );
    RegisterScriptDictionary( CScriptMgr::Instance().getEnginePtr() );
    RegisterScriptMath( CScriptMgr::Instance().getEnginePtr() );
    NScriptSize::Register();
    NScriptGlobals::Register();
    NScriptColor::Register();
    NScriptPoint::Register();
    NScriptBitmask::Register();
    NScriptEvent::Register();
    NScriptActionManager::Register();
    NScriptCamera::Register();
    NScriptCameraManager::Register();
    NScriptSprite::Register();
    NScriptSound::Register();
    NScriptObjectDataManager::Register();
    NScriptSettings::Register();
    NScriptHighResolutionTimer::Register();
    NScriptUIControl::Register();
    NScriptMenu::Register();
    NScriptMenuManager::Register();
    NScriptStrategy::Register();
    NScriptFontManager::Register();
    NScriptScriptManager::Register();
    NScriptDevice::Register();
    NScriptVisual::Register();
    NScriptPhysics2d::Register();
    NScriptStatCounter::Register();
    
    // Register game level functions
    registerGameFunc();
    
    // Load the script list table
    CScriptMgr::Instance().loadListTable( CSettings::Instance().getScriptListTable() );

    // Load the starting group
    CScriptMgr::Instance().loadGroup( CSettings::Instance().getScriptGroup() );
    CScriptMgr::Instance().prepare(
        CSettings::Instance().getScriptGroup(), CSettings::Instance().getScriptMain() );

    CHighResTimer::Instance().calcElapsedTime();
}

/***************************************************************************
*   DESC:  Register game functions
****************************************************************************/
void CGame::registerGameFunc()
{
    using namespace NScriptGlobals; // Used for Throw
        
    asIScriptEngine * pEngine = CScriptMgr::Instance().getEnginePtr();

    Throw( pEngine->RegisterGlobalFunction("void PollEvents()", WRAP_MFN(CGame, pollEvents), asCALL_GENERIC, this) );
}

/***************************************************************************
*   DESC:  Poll for game events
****************************************************************************/
void CGame::pollEvents()
{
    // Event handler
    SDL_Event msgEvent;

    // Clear the stored event queue
    CActionMgr::Instance().clearQueue();

    // Handle events on queue
    while( SDL_PollEvent( &msgEvent ) )
    {
        // Add the event to the event queue for message handling in scripts
        CActionMgr::Instance().queueEvent( msgEvent );
        
        handleEvent( msgEvent );
    }
}

/***************************************************************************
*    decs:  Record the command buffer vector in the device
*           for all the sprite objects that are to be rendered
****************************************************************************/
void CGame::recordCommandBuffer( uint32_t cmdBufIndex )
{
    CStrategyMgr::Instance().recordCommandBuffer( cmdBufIndex );
    CMenuMgr::Instance().recordCommandBuffer( cmdBufIndex );

    CStrategyMgr::Instance().updateSecondaryCmdBuf( cmdBufIndex );
    CMenuMgr::Instance().updateSecondaryCmdBuf( cmdBufIndex );
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
    // Handle events for the menu manager
    CMenuMgr::Instance().handleEvent( rEvent );

    // Filter out these events. Can't do this through the normal event filter
    if( (rEvent.type >= SDL_EVENT_JOYSTICK_AXIS_MOTION) && (rEvent.type <= SDL_EVENT_JOYSTICK_BUTTON_UP) )
        return false;

    else if( rEvent.type == SDL_EVENT_GAMEPAD_ADDED )
        CDevice::Instance().addGamepad( rEvent.cdevice.which );

    else if( rEvent.type == SDL_EVENT_GAMEPAD_REMOVED )
        CDevice::Instance().removeGamepad( rEvent.cdevice.which );

    else if( rEvent.type == SDL_EVENT_LOW_MEMORY )
        displayErrorMsg( "Low Memory Error", "The device is experiencing low memory. Try freeing up some apps." );

    // In a traditional game, want the pause menu to display when the game is sent to the background
    else if( (rEvent.type == SDL_EVENT_WILL_ENTER_BACKGROUND) && !CMenuMgr::Instance().isMenuActive() )
        NGenFunc::DispatchEvent( NMenuEvent::ESCAPE_ACTION );

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
bool FilterEvents( void * userdata, SDL_Event * pEvent )
{
    // Return false to indicate that the event should be dropped from the internal event queue

    // Do our own deadzone filtering
    if( pEvent->type == SDL_EVENT_GAMEPAD_AXIS_MOTION )
    {
        // Analog stick max values -32768 to 32767
        const int deadZone = CSettings::Instance().getGamePadStickDeadZone() *
            defs_ANALOG_PERCENTAGE_CONVERTION;

        if( ((pEvent->gaxis.axis >= SDL_GAMEPAD_AXIS_LEFTX) &&
            (pEvent->gaxis.axis <= SDL_GAMEPAD_AXIS_RIGHTY)) &&
            (((pEvent->gaxis.value >= 0) && (pEvent->gaxis.value < deadZone)) ||
            ((pEvent->gaxis.value <= 0) && (pEvent->gaxis.value > -deadZone))) )
            return false;
    }

    // Return true to indicate that the event should be added to the internal event queue
    return true;
}
