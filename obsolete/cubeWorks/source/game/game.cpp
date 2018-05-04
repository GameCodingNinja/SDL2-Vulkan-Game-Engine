
/************************************************************************
*    FILE NAME:       game.cpp
*
*    DESCRIPTION:     CGame class
************************************************************************/

// Physical component dependency
#include "game.h"

// Boost lib dependencies
#include <boost/bind.hpp>
#include <boost/format.hpp>

// Game lib dependencies
#include <system/device.h>
#include <managers/signalmanager.h>
#include <gui/menumanager.h>
#include <gui/uicontrol.h>
#include <gui/menu.h>
#include <utilities/exceptionhandling.h>
#include <utilities/settings.h>

// Game dependencies
#include "../state/startupstate.h"
#include "../state/titlescreenstate.h"
#include "../state/loadstate.h"
#include "../state/runstate.h"
#include "../smartGUI/smartconfirmbtn.h"
#include "../smartGUI/smartresolutionbtn.h"
#include "../smartGUI/smartapplysettingsbtn.h"
#include "../smartGUI/smartfullscreencheckbox.h"
#include "../smartGUI/smartvsynccheckbox.h"
#include "../smartGUI/smartdeadzoneslider.h"
#include "../smartGUI/smartkeybindbtn.h"

/************************************************************************
*    desc:  Constructer
************************************************************************/
CGame::CGame()
{
    CSignalMgr::Instance().Connect_SmartGui( boost::bind(&CGame::SmartGuiControlCreate, this, _1) );
    CSignalMgr::Instance().Connect_SmartMenu( boost::bind(&CGame::SmartMenuCreate, this, _1) );

}   // constructor


/************************************************************************
*    desc:  destructer                                                             
************************************************************************/
CGame::~CGame()
{
}   // destructer


/************************************************************************
*    desc:  Init the game
************************************************************************/
void CGame::Init()
{
    CBaseGame::Init();

    // Setup the message filtering
    SDL_SetEventFilter(FilterEvents, 0);
    
    // Handle some events on startup
    PollEvents();

    // Create the startup state
    upGameState.reset( new CStartUpState );

    // Let the games begin
    StartGame();

}   // Init


/************************************************************************
*    desc:  Callback for when a smart gui control is created
************************************************************************/
void CGame::SmartGuiControlCreate( CUIControl * pUIControl )
{
    if( pUIControl->GetFaction() == "decision_btn" )
        pUIControl->SetSmartGui( new CSmartConfirmBtn( pUIControl ) );
    
    else if( pUIControl->GetFaction() == "key_binding_btn" )
        pUIControl->SetSmartGui( new CSmartKeyBindBtn( pUIControl ) );

    else if( pUIControl->GetName() == "resolution_btn_lst" )
        pUIControl->SetSmartGui( new CSmartResolutionBtn( pUIControl ) );

    else if( pUIControl->GetName() == "settings_apply_btn" )
        pUIControl->SetSmartGui( new CSmartApplySettingsBtn( pUIControl ) );

    else if( pUIControl->GetName() == "full_screen_check_box" )
        pUIControl->SetSmartGui( new CSmartScrnCheckBox( pUIControl ) );

    else if( pUIControl->GetName() == "v-sync_check_box" )
            pUIControl->SetSmartGui( new CSmartVSyncCheckBox( pUIControl ) );

    else if( pUIControl->GetName() == "settings_dead_zone_slider" )
            pUIControl->SetSmartGui( new CSmartDeadZoneSlider( pUIControl ) );

}   // SmartGuiControlCreate


/************************************************************************
*    desc:  Callback for when a smart menu is created
************************************************************************/
void CGame::SmartMenuCreate( CMenu * pMenu )
{

}   // SmartMenuCreate


/***************************************************************************
*    decs:  Handle the state change
****************************************************************************/
void CGame::DoStateChange()
{
    if( upGameState->DoStateChange() )
    {
        // Get the game state we are moving to
        const EGameState curState = upGameState->GetState();

        // Get the game state we are moving to
        const EGameState nextState = upGameState->GetNextState();

        // Get the message to the next state
        const CStateMessage stateMessage = upGameState->GetStateMessage();
        
        // Allow for some message processing after load states
        if( (curState == EGS_STARTUP) || (curState == EGS_GAME_LOAD) )
        {
            upGameState.reset();
            PollEvents();
        }
        
        if( nextState == EGS_TITLE_SCREEN )
            upGameState.reset( new CTitleScreenState );

        else if( nextState == EGS_GAME_LOAD )
            upGameState.reset( new CLoadState( stateMessage ) );

        else if( nextState == EGS_RUN )
            upGameState.reset( new CRunState );

        else
            throw NExcept::CCriticalException("Error Invalid game state",
                boost::str( boost::format("Next state not valid (cur %d, next %d).\n\n%s\nLine: %s") % curState % nextState % __FUNCTION__ % __LINE__ ));
    }

}   // DoStateChange */


/************************************************************************
*    desc:  Handle events
************************************************************************/
bool CGame::HandleEvent( const SDL_Event & rEvent )
{
    if( (rEvent.type == SDL_QUIT) || (rEvent.type == SDL_APP_TERMINATING) )
        return true;

    // Filter out these events. Can't do this through the normal event filter
    if( (rEvent.type >= SDL_JOYAXISMOTION) && (rEvent.type <= SDL_JOYBUTTONUP) )
        return false;

    else if( rEvent.type == SDL_CONTROLLERDEVICEADDED )
        CDevice::Instance().AddGamepad( rEvent.cdevice.which );

    else if( rEvent.type == SDL_CONTROLLERDEVICEREMOVED )
        CDevice::Instance().RemoveGamepad( rEvent.cdevice.which );

    // Handle events
    if( upGameState )
        upGameState->HandleEvent( rEvent );

    return false;

}   // HandleEvent


/************************************************************************
*    desc:  Handle any misc processing before the real work is started.
************************************************************************/
void CGame::MiscProcess()
{
}   // MiscProcess


/************************************************************************
*    desc:  Handle the physics.
************************************************************************/
void CGame::Physics()
{
    upGameState->Physics();
    
}   // Physics


/***************************************************************************
*    decs:  Update animations
****************************************************************************/
void CGame::Update()
{
    upGameState->Update();

}   // Update


/***************************************************************************
*    desc:  Transform game objects
****************************************************************************/
void CGame::Transform()
{
    upGameState->Transform();

}   // Transform


/***************************************************************************
*    decs:  3D/2D Render of game content
****************************************************************************/
void CGame::PreRender()
{
    upGameState->PreRender();

}   // GameRender3D


/***************************************************************************
*    decs:  3D/2D Render of content after post process effects
****************************************************************************/
void CGame::PostRender()
{
    upGameState->PostRender();

}   // PostGameRender2D


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
        const int deadZone = CSettings::Instance().GetGamePadStickDeadZone() * 
            NDefs::ANALOG_PERCENTAGE_CONVERTION;

        if( ((pEvent->caxis.axis >= SDL_CONTROLLER_AXIS_LEFTX) &&
            (pEvent->caxis.axis <= SDL_CONTROLLER_AXIS_RIGHTY)) &&
            (((pEvent->caxis.value >= 0) && (pEvent->caxis.value < deadZone)) ||
            ((pEvent->caxis.value <= 0) && (pEvent->caxis.value > -deadZone))) )
            return 0;
    }

    // Return 1 to indicate that the event should stay in the event queue
    return 1;

}   // FilterEvents
