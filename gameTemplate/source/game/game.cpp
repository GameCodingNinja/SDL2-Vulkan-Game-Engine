
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
#include <managers/shadermanager.h>
#include <gui/menumanager.h>
#include <gui/uicontrol.h>
#include <gui/menu.h>
#include <utilities/exceptionhandling.h>
#include <utilities/settings.h>
#include <utilities/statcounter.h>
#include <common/isprite.h>
#include <common/color.h>
#include <common/build_defs.h>
#include <objectdata/objectdatamanager.h>
#include <slot/betmanager.h>

// Boost lib dependencies
#include <boost/bind.hpp>
#include <boost/format.hpp>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CGame::CGame()
{
    CSignalMgr::Instance().connect_smartGui( boost::bind(&CGame::smartGuiControlCreateCallBack, this, _1) );
    CSignalMgr::Instance().connect_smartMenu( boost::bind(&CGame::smartMenuCreateCallBack, this, _1) );
    CSignalMgr::Instance().connect_aICreate( boost::bind(&CGame::aICreateCallBack, this, _1, _2) );
    CShaderMgr::Instance().connect_initShader( boost::bind(&CGame::shaderInitCallBack, this, _1) );

    if( NBDefs::IsDebugMode() )
        CStatCounter::Instance().connect( boost::bind(&CGame::statStringCallBack, this, _1) );
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CGame::~CGame()
{
}


/************************************************************************
*    DESC:  Init the game
************************************************************************/
void CGame::init()
{
    CBaseGame::init();

    // Setup the message filtering
    //SDL_SetEventFilter(FilterEvents, 0);

    // Handle some events on startup
    pollEvents();

    // Init with the total amount of credits
    CBetMgr::Instance().setCredits( 50000 );

    // Create the startup state
    upGameState.reset( new CStartUpState );
    upGameState->init();

    // Let the games begin
    startGame();
}


/************************************************************************
*    DESC:  Callback for when a smart gui control is created
************************************************************************/
void CGame::smartGuiControlCreateCallBack( CUIControl * pUIControl )
{
    if( pUIControl->getFaction() == "decision_btn" )
        pUIControl->setSmartGui( new CSmartConfirmBtn( pUIControl ) );

    else if( pUIControl->getFaction() == "key_binding_btn" )
        pUIControl->setSmartGui( new CSmartKeyBindBtn( pUIControl ) );

    else if( pUIControl->getName() == "resolution_btn_lst" )
        pUIControl->setSmartGui( new CSmartResolutionBtn( pUIControl ) );

    else if( pUIControl->getName() == "settings_apply_btn" )
        pUIControl->setSmartGui( new CSmartApplySettingsBtn( pUIControl ) );

    else if( pUIControl->getName() == "full_screen_check_box" )
        pUIControl->setSmartGui( new CSmartScrnCheckBox( pUIControl ) );

    else if( pUIControl->getName() == "v-sync_check_box" )
            pUIControl->setSmartGui( new CSmartVSyncCheckBox( pUIControl ) );

    else if( pUIControl->getName() == "settings_dead_zone_slider" )
            pUIControl->setSmartGui( new CSmartDeadZoneSlider( pUIControl ) );
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
void CGame::aICreateCallBack( const std::string & aiName, iSprite * pSprite )
{
    if( aiName == "aiBall" )
        pSprite->setAI( new CBallAI( pSprite ) );
}


/************************************************************************
*    DESC:  Callback for shader init
************************************************************************/
void CGame::shaderInitCallBack( const std::string & shaderId )
{
    // Init the color for fading in
    CShaderMgr::Instance().setShaderColor( shaderId, "additive", CColor(0,0,0,1) );
}


/************************************************************************
*    DESC:  Callback for the state string
************************************************************************/
void CGame::statStringCallBack( const std::string & statStr )
{
    if( !CSettings::Instance().getFullScreen() )
        SDL_SetWindowTitle( m_pWindow, statStr.c_str() );
}


/***************************************************************************
*    decs:  Handle the state change
****************************************************************************/
void CGame::doStateChange()
{
    if( upGameState->doStateChange() )
    {
        // Get the game state we are moving to
        const NGameDefs::EGameState curState = upGameState->getState();

        // Get the game state we are moving to
        const NGameDefs::EGameState nextState = upGameState->getNextState();

        // Get the message to the next state
        const CStateMessage stateMessage = upGameState->getStateMessage();

        // Free the current state to ensure no messages will be processed by a state
        upGameState.reset();

        // Process any lingering messages so that the new state isn't
        // getting hammered by a bunch of queued up messages
        pollEvents();

        if( nextState == NGameDefs::EGS_TITLE_SCREEN )
            upGameState.reset( new CTitleScreenState );

        else if( nextState == NGameDefs::EGS_GAME_LOAD )
            upGameState.reset( new CLoadState( stateMessage ) );

        else if( nextState == NGameDefs::EGS_RUN )
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
        NGenFunc::DispatchEvent( NMenu::EGE_MENU_ESCAPE_ACTION );

    // Handle events
    if( upGameState )
        upGameState->handleEvent( rEvent );

    return false;
}


/************************************************************************
*    DESC:  Handle any misc processing before the real work is started.
************************************************************************/
void CGame::miscProcess()
{
    upGameState->miscProcess();
}


/************************************************************************
*    DESC:  Handle the physics.
************************************************************************/
void CGame::physics()
{
    upGameState->physics();
}


/***************************************************************************
*    decs:  Update animations
****************************************************************************/
void CGame::update()
{
    upGameState->update();
}


/***************************************************************************
*    DESC:  Transform game objects
****************************************************************************/
void CGame::transform()
{
    upGameState->transform();
}


/***************************************************************************
*    decs:  Render of game content
****************************************************************************/
void CGame::render()
{
    upGameState->preRender();
    upGameState->postRender();
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
