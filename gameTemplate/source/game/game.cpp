
/************************************************************************
*    FILE NAME:       game.cpp
*
*    DESCRIPTION:     CGame class
************************************************************************/

// Physical component dependency
#include "game.h"

// Game dependencies
#include "../state/startupstate.h"
#include "../state/titlescreenstate.h"
#include "../state/loadstate.h"
#include "../state/runstate.h"
#include "../ai/ballai.h"
#include "../smartGUI/smartconfirmbtn.h"
#include "../smartGUI/smartresolutionbtn.h"
#include "../smartGUI/smartapplysettingsbtn.h"
#include "../smartGUI/smartfullscreencheckbox.h"
#include "../smartGUI/smartvsynccheckbox.h"
#include "../smartGUI/smartdeadzoneslider.h"
#include "../smartGUI/smartkeybindbtn.h"

// Game lib dependencies
#include <system/device.h>
#include <managers/signalmanager.h>
#include <managers/cameramanager.h>
#include <gui/menumanager.h>
#include <gui/icontrol.h>
#include <gui/menu.h>
#include <utilities/exceptionhandling.h>
#include <utilities/settings.h>
#include <utilities/statcounter.h>
#include <utilities/highresolutiontimer.h>
#include <sprite/sprite.h>

// Boost lib dependencies
#include <boost/bind.hpp>
#include <boost/format.hpp>

// SDL lib dependencies
#include <SDL.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CGame::CGame() :
    m_gameRunning(false)
{
    // Init the device. NOTE: This always needs to be first
    // This call loads the settings XML
    CDevice::Instance().init( std::bind( &CGame::recordCommandBuffer, this, std::placeholders::_1) );

    // Load the camera data early because many objects init the default camera in their constructor
    // The above call to CDevice::Instance().init loads the settings which the camera relies on
    CCameraMgr::Instance().load( "data/objects/camera.lst" );
    
    CSignalMgr::Instance().connect_smartGui( boost::bind(&CGame::smartGuiControlCreateCallBack, this, _1) );
    CSignalMgr::Instance().connect_smartMenu( boost::bind(&CGame::smartMenuCreateCallBack, this, _1) );
    CSignalMgr::Instance().connect_aICreate( boost::bind(&CGame::aICreateCallBack, this, _1, _2) );

    if( CSettings::Instance().isDebugMode() )
        CStatCounter::Instance().connect( boost::bind(&CGame::statStringCallBack, this, _1) );
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CGame::~CGame()
{
    // Free all objects
    // This waits for idle before returning
    upGameState.reset();
    
    // Free the menu assets
    CMenuMgr::Instance().clear();
    
    // Destroy the window and Vulkan instance
    CDevice::Instance().destroy();
}


/************************************************************************
*    DESC:  Create the game
************************************************************************/
void CGame::create()
{
    // Create the device
    CDevice::Instance().create( "data/shaders/pipeline.cfg" );
    
    // Show the window
    CDevice::Instance().showWindow( true );

    // Setup the message filtering
    SDL_SetEventFilter(FilterEvents, 0);

    // Handle some events on startup
    pollEvents();

    // Calculate the ratio after a bit of startup
    CSettings::Instance().calcRatio();

    // Create the startup state
    upGameState.reset( new CStartUpState );
    upGameState->init();

    // Let the games begin
    m_gameRunning = true;
}


/************************************************************************
*    DESC:  Callback for when a smart gui control is created
************************************************************************/
void CGame::smartGuiControlCreateCallBack( iControl * piControl )
{
    if( piControl->getFaction() == "decision_btn" )
        piControl->setSmartGui( new CSmartConfirmBtn( piControl ) );

    else if( piControl->getFaction() == "key_binding_btn" )
        piControl->setSmartGui( new CSmartKeyBindBtn( piControl ) );

    else if( piControl->getName() == "resolution_btn_lst" )
        piControl->setSmartGui( new CSmartResolutionBtn( piControl ) );

    else if( piControl->getName() == "settings_apply_btn" )
        piControl->setSmartGui( new CSmartApplySettingsBtn( piControl ) );

    else if( piControl->getName() == "full_screen_check_box" )
        piControl->setSmartGui( new CSmartScrnCheckBox( piControl ) );

    else if( piControl->getName() == "v-sync_check_box" )
        piControl->setSmartGui( new CSmartVSyncCheckBox( piControl ) );

    else if( piControl->getName() == "settings_dead_zone_slider" )
        piControl->setSmartGui( new CSmartDeadZoneSlider( piControl ) );
}


/************************************************************************
*    DESC:  Callback for when a smart menu is created
************************************************************************/
void CGame::smartMenuCreateCallBack( CMenu * pMenu )
{
}


/***************************************************************************
*    decs:  Call back function to create sprite ai
****************************************************************************/
void CGame::aICreateCallBack( const std::string & aiName, CSprite * pSprite )
{
    if( aiName == "aiBall" )
        pSprite->setAI( new CBallAI( pSprite ) );
}


/************************************************************************
*    DESC:  Callback for the state string
************************************************************************/
void CGame::statStringCallBack( const std::string & statStr )
{
    #if !defined(__ANDROID__)
    if( !CSettings::Instance().getFullScreen() )
        CDevice::Instance().setWindowTitle( statStr );
    #else
    NGenFunc::PostDebugMsg(statStr);
    #endif
}


/***************************************************************************
*    decs:  Handle the state change
****************************************************************************/
void CGame::doStateChange()
{
    if( upGameState->doStateChange() )
    {
        // Wait for all rendering to be finished
        CDevice::Instance().waitForIdle();
    
        // Get the game state we are moving to
        const NStateDefs::EGameState curState = upGameState->getState();

        // Get the game state we are moving to
        const NStateDefs::EGameState nextState = upGameState->getNextState();

        // Get the message to the next state
        const CStateMessage stateMessage = upGameState->getStateMessage();

        // Free the current state to ensure no messages will be processed by a state
        upGameState.reset();

        // Process any lingering messages so that the new state isn't
        // getting hammered by a bunch of queued up messages
        pollEvents();

        if( nextState == NStateDefs::EGS_TITLE_SCREEN )
            upGameState.reset( new CTitleScreenState );

        else if( nextState == NStateDefs::EGS_GAME_LOAD )
            upGameState.reset( new CLoadState( stateMessage ) );

        else if( nextState == NStateDefs::EGS_RUN )
            upGameState.reset( new CRunState );

        else
            throw NExcept::CCriticalException("Error Invalid game state",
                boost::str( boost::format("Next state not valid (cur %d, next %d).\n\n%s\nLine: %s")
                    % curState % nextState % __FUNCTION__ % __LINE__ ));

        // Do any pre-game loop init's
        upGameState->init();
    }
}


/************************************************************************
*    DESC:  Handle events
************************************************************************/
bool CGame::handleEvent( const SDL_Event & rEvent )
{
    if( (rEvent.type == SDL_QUIT) || (rEvent.type == SDL_APP_TERMINATING) )
        return true;

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

    // Handle events
    if( upGameState )
        upGameState->handleEvent( rEvent );

    return false;
}


/***************************************************************************
*    decs:  Record the command buffer vector in the device
*           for all the sprite objects that are to be rendered
****************************************************************************/
void CGame::recordCommandBuffer( uint32_t cmdBufIndex )
{
    upGameState->recordCommandBuffer( cmdBufIndex );
}


/***************************************************************************
*   DESC:  Main game loop
****************************************************************************/
bool CGame::gameLoop()
{
    // Handle the state change
    doStateChange();

    // Poll for game events
    pollEvents();

    // Get our elapsed time
    CHighResTimer::Instance().calcElapsedTime();

    if( m_gameRunning )
    {
        // Handle the physics
        upGameState->physics();

        // Update animations, Move sprites, Check for collision
        upGameState->update();

        // Transform game objects
        upGameState->transform();

        // Do the rendering
        CDevice::Instance().render();

        // Inc the stat cycle
        if( CSettings::Instance().isDebugMode() )
            CStatCounter::Instance().incCycle();
    }

    return m_gameRunning;
}


/***************************************************************************
*   DESC:  Poll for game events
****************************************************************************/
void CGame::pollEvents()
{
    // Event handler
    SDL_Event msgEvent;

    // Handle events on queue
    while( SDL_PollEvent( &msgEvent ) )
    {
        // let the game handle the event
        // turns true on quit
        if( handleEvent( msgEvent ) )
        {
            // Stop the game
            m_gameRunning = false;

            // Hide the window to give the impression of a quick exit
            CDevice::Instance().showWindow( false );

            break;
        }
    }
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

        if( std::abs(pEvent->caxis.value) < deadZone )
            return 0;
    }

    // Return 1 to indicate that the event should stay in the event queue
    return 1;
}
