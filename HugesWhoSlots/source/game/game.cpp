
/************************************************************************
*    FILE NAME:       game.cpp
*
*    DESCRIPTION:     CGame class
************************************************************************/

// Physical component dependency
#include "game.h"

// Game dependencies
#include "gamesave.h"
#include "../state/startupstate.h"
#include "../state/lobbystate.h"
#include "../state/bigpaybackstate.h"
#include "../state/loadstate.h"
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
#include <managers/fontmanager.h>
#include <managers/cameramanager.h>
#include <gui/menumanager.h>
#include <gui/uicontrol.h>
#include <gui/menu.h>
#include <gui/uicheckbox.h>
#include <utilities/exceptionhandling.h>
#include <utilities/settings.h>
#include <utilities/statcounter.h>
#include <common/color.h>
#include <common/size.h>
#include <common/point.h>
#include <common/build_defs.h>
#include <2d/sprite2d.h>
#include <objectdata/objectdatamanager.h>
#include <slot/betmanager.h>

// Boost lib dependencies
#include <boost/bind.hpp>
#include <boost/format.hpp>

// SDL lib dependencies
#include <SDL.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CGame::CGame()
{
    CSignalMgr::Instance().connect_smartGui( boost::bind(&CGame::smartGuiControlCreateCallBack, this, _1) );
    CSignalMgr::Instance().connect_smartMenu( boost::bind(&CGame::smartMenuCreateCallBack, this, _1) );
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

    // Load the game save data
    CGameSave::Instance().load();

    // Init with the total amount of credits
    CBetMgr::Instance().setCredits( CGameSave::Instance().getTotalCredits() );

    // Create the startup state
    upGameState.reset( new CStartUpState );
    upGameState->init();

    // Setup the sprite to be used as a debug string
    if( NBDefs::IsDebugMode() )
    {
        // Debug output - Object data list should be loaded at this point
        CObjectDataMgr::Instance().loadGroup2D( "(debug)" );
        upDebugDisplay.reset( new CSprite2D( CObjectDataMgr::Instance().getData2D("(debug)", "debugString_font") ) );
        const CSize<float> & rSize = CSettings::Instance().getDefaultSizeHalf();
        CFontMgr::Instance().load( "data/textures/fonts/font_debug.lst" );
        upDebugDisplay->setPos( CPoint<float>( -(rSize.w-10), rSize.h-10 ) );
        upDebugDisplay->getVisualComponent().setFontProperties( CFontProperties("dejavu_sans_reg_outline_24", NDefs::EHA_HORZ_LEFT, NDefs::EVA_VERT_TOP) );
        upDebugDisplay->transform();
    }

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

    else if( pUIControl->getName() == "lobby_music_check_box" )
    {
        NGenFunc::DynCast<CUICheckBox>(pUIControl)->setToggleState( CGameSave::Instance().getPlayLobbyMusic() );

        pUIControl->connect_executionAction( boost::bind(&CGame::lobbyMusicCallBack, this, _1) );
    }
    else if( pUIControl->getName() == "spin_music_check_box" )
    {
        NGenFunc::DynCast<CUICheckBox>(pUIControl)->setToggleState( CGameSave::Instance().getPlaySpinMusic() );

        pUIControl->connect_executionAction( boost::bind(&CGame::spinMusicCallBack, this, _1) );
    }
    else if( pUIControl->getName() == "stop_sounds_check_box" )
    {
        NGenFunc::DynCast<CUICheckBox>(pUIControl)->setToggleState( CGameSave::Instance().getPlayStopSounds() );

        pUIControl->connect_executionAction( boost::bind(&CGame::stopSoundsCallBack, this, _1) );
    }

    // Dev Debug call backs
    else if( pUIControl->getName() == "dev-option-v-sync_check_box" )
    {
        NGenFunc::DynCast<CUICheckBox>(pUIControl)->setToggleState( CSettings::Instance().getVSync() );

        pUIControl->connect_executionAction( boost::bind(&CGame::devDebugVSyncCallBack, this, _1) );
    }
    else if( pUIControl->getName() == "debug_string_check_box" )
    {
        NGenFunc::DynCast<CUICheckBox>(pUIControl)->setToggleState( CSettings::Instance().getDebugStrVisible() );

        pUIControl->connect_executionAction( boost::bind(&CGame::devDebugStringCallBack, this, _1) );
    }
}


/************************************************************************
*    DESC:  Callback for when a smart menu is created
************************************************************************/
void CGame::smartMenuCreateCallBack( CMenu * pMenu )
{
}


/************************************************************************
*    DESC:  Game settings call back fuctions
************************************************************************/
void CGame::lobbyMusicCallBack( CUIControl * pUIControl )
{
    auto pCtrl = NGenFunc::DynCast<CUICheckBox>(pUIControl);
    CGameSave::Instance().setPlayLobbyMusic( pCtrl->getToggleState() );
    CGameSave::Instance().openSaveClose();

    if( (upGameState != nullptr) && (upGameState->getState() == NGameDefs::EGS_LOBBY) )
        upGameState->allowMusic( pCtrl->getToggleState() );

}   // LobbyMusicCallBack

void CGame::spinMusicCallBack( CUIControl * pUIControl )
{
    auto pCtrl = NGenFunc::DynCast<CUICheckBox>(pUIControl);
    CGameSave::Instance().setPlaySpinMusic( pCtrl->getToggleState() );
    CGameSave::Instance().openSaveClose();

    if( (upGameState != nullptr) && (upGameState->getState() != NGameDefs::EGS_LOBBY) )
        upGameState->allowMusic( pCtrl->getToggleState() );

}   // SpinMusicCallBack

void CGame::stopSoundsCallBack( CUIControl * pUIControl )
{
    auto pCtrl = NGenFunc::DynCast<CUICheckBox>(pUIControl);
    CGameSave::Instance().setPlayStopSounds( pCtrl->getToggleState() );
    CGameSave::Instance().openSaveClose();

    if( (upGameState != nullptr) && (upGameState->getState() != NGameDefs::EGS_LOBBY) )
        upGameState->allowStopSounds( pCtrl->getToggleState() );
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
    if( NBDefs::IsMobileDevice() )
    {
        if( CSettings::Instance().getDebugStrVisible() && upDebugDisplay )
            upDebugDisplay->getVisualComponent().createFontString( statStr );
    }
    else
    {
        if( !CSettings::Instance().getFullScreen() )
            SDL_SetWindowTitle( m_pWindow, statStr.c_str() );
        else
            upDebugDisplay->getVisualComponent().createFontString( statStr );
    }
}


/************************************************************************
*    DESC:  Dev debug settings call back fuctions
************************************************************************/
void CGame::devDebugVSyncCallBack( CUIControl * pUIControl )
{
    auto pCtrl = NGenFunc::DynCast<CUICheckBox>(pUIControl);

    CDevice::Instance().enableVSync( pCtrl->getToggleState() );
}

void CGame::devDebugStringCallBack( CUIControl * pUIControl )
{
    auto pCtrl = NGenFunc::DynCast<CUICheckBox>(pUIControl);

    CSettings::Instance().setDebugStrVisible( pCtrl->getToggleState() );
}


/***************************************************************************
*    decs:  Handle the state change
****************************************************************************/
void CGame::doStateChange()
{
    if( upGameState->doStateChange() )
    {
        // Get the game state we are currently in
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

        if( nextState == NGameDefs::EGS_LOBBY )
            upGameState.reset( new CLobbyState("(lobby)") );

        else if( nextState == NGameDefs::EGS_BIG_CASH_BACK )
            upGameState.reset( new CBigPayBackState("(big_pay_back)") );

        else if( nextState == NGameDefs::EGS_GAME_LOAD )
            upGameState.reset( new CLoadState( stateMessage ) );

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

    else if( rEvent.type == SDL_WINDOWEVENT )
    {
        if( (rEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) && upDebugDisplay )
        {
            const CSize<float> & rSize = CSettings::Instance().getDefaultSizeHalf();
            upDebugDisplay->setPos( CPoint<float>( -(rSize.w-5), rSize.h-7 ) );
            upDebugDisplay->transform();
        }
    }

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
*    decs:  3D/2D Render of content after post process effects
****************************************************************************/
void CGame::render()
{
    upGameState->preRender();

    upGameState->postRender();

    if( NBDefs::IsDebugMode() )
    {
        if( NBDefs::IsMobileDevice() )
        {
            if( CSettings::Instance().getDebugStrVisible() && upDebugDisplay )
                upDebugDisplay->render( CCameraMgr::Instance().getDefaultProjMatrix() );
        }
        else
        {
            if( CSettings::Instance().getFullScreen() )
                upDebugDisplay->render( CCameraMgr::Instance().getDefaultProjMatrix() );
        }
    }
}


/***************************************************************************
*    decs:  Filter out events we don't want
****************************************************************************/
int FilterEvents( void * userdata, SDL_Event * pEvent )
{
    // Return 0 to indicate that the event should be removed from the event queue

    // Do our own dead zone filtering
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
