
/************************************************************************
*    FILE NAME:       smartapplysettingsbtn.cpp
*
*    DESCRIPTION:     Class CSmartExitBtn
************************************************************************/

// Physical component dependency
#include "smartapplysettingsbtn.h"

// Game dependencies local
#include "smartresolutionbtn.h"

// Game dependencies
#include "../game/game.h"

// Game lib dependencies
#include <system/device.h>
#include <gui/menumanager.h>
#include <gui/uicheckbox.h>
#include <gui/uibuttonlist.h>
#include <gui/menu.h>
#include <gui/uislider.h>
#include <common/defs.h>
#include <utilities/genfunc.h>
#include <utilities/settings.h>
#include <managers/cameramanager.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSmartApplySettingsBtn::CSmartApplySettingsBtn( iControl * piControl ) :
    CSmartSettingsMenuBtn( piControl )
{
}


/***************************************************************************
*    DESC:  Handle events
****************************************************************************/
void CSmartApplySettingsBtn::handleEvent( const SDL_Event & rEvent )
{
    if( rEvent.type == NMenuDefs::EME_MENU_TRANS_IN )
        m_piControl->changeState(NUIControlDefs::ECS_DISABLE);
}


/***************************************************************************
*    DESC:  Called when the control is executed - Applies the settings
****************************************************************************/
void CSmartApplySettingsBtn::execute()
{
    bool resChanged(false), fullScreenChanged(false);
    bool settingsChangeMade(false);

    // Get the settings menu
    CMenu & rMenu = CMenuMgr::Instance().getActiveMenu();

    // Check for the resolution button list
    CSmartResolutionBtn & rSmartRes = (*dynamic_cast<CSmartResolutionBtn *>(rMenu.getPtrToControl( "resolution_btn_lst" )->getSmartGuiPtr()));
    if( rSmartRes.wasResolutionChanged() )
    {
        rSmartRes.setResolutionChange();
        resChanged = true;
        settingsChangeMade = true;
    }
    
    // Get the control
    iControl * pControl;

    // Check for the full screen check box
    pControl = rMenu.getPtrToControl( "full_screen_check_box" );
    const bool fullScreenState = pControl->getToggleState();
    const bool oldFullScreenState = CSettings::Instance().getFullScreen();
    if( CSettings::Instance().getFullScreen() != fullScreenState )
    {
        CSettings::Instance().setFullScreen( fullScreenState );
        fullScreenChanged = true;
        settingsChangeMade = true;
    }

    // Check for the v-sync check box
    pControl = rMenu.getPtrToControl( "v-sync_check_box" );
    const bool vsyncState = pControl->getToggleState();
    if( CSettings::Instance().getVSync() != vsyncState )
    {
        CSettings::Instance().setVSync( vsyncState );
        CDevice::Instance().enableVSync( vsyncState );
        settingsChangeMade = true;
    }

    // Check for dead zone slider
    pControl = rMenu.getPtrToControl( "settings_dead_zone_slider" );
    int sliderValue = pControl->getSliderPosValue();
    if( CSettings::Instance().getGamePadStickDeadZone() != sliderValue )
    {
        CSettings::Instance().setGamePadStickDeadZone( sliderValue );
        settingsChangeMade = true;
    }

    if( resChanged )
    {
        // SDL2 doesn't allow res change in full screen
        // so take us out of full screen mode for res changes
        if( oldFullScreenState && fullScreenState )
            CDevice::Instance().setFullScreen( false );

        SDL_DisplayMode mode;
        SDL_GetCurrentDisplayMode(0, &mode);

        mode.w = CSettings::Instance().getSize().getW();
        mode.h = CSettings::Instance().getSize().getH();
        mode.refresh_rate = 0;

        SDL_SetWindowDisplayMode( CDevice::Instance().getWindow(), &mode );

        if( fullScreenChanged || fullScreenState )
            CDevice::Instance().setFullScreen( fullScreenState );

        SDL_SetWindowSize(
            CDevice::Instance().getWindow(),
            CSettings::Instance().getSize().getW(),
            CSettings::Instance().getSize().getH() );

        SDL_SetWindowPosition(
            CDevice::Instance().getWindow(),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED );

        // Reset the transform to recalculate mouse collision
        CMenuMgr::Instance().resetTransform();

        // Reset the dynamic positions of menus
        CMenuMgr::Instance().resetDynamicOffset();

        // Rebuild all the camera projection matrixes
        CCameraMgr::Instance().rebuildProjectionMatrix();
    }
    else if( fullScreenChanged )
    {
        CDevice::Instance().setFullScreen( fullScreenState );
    }

    // Only save if a change was actually made
    if( settingsChangeMade )
        CSettings::Instance().saveSettings();

    m_piControl->changeState(NUIControlDefs::ECS_DISABLE);

    // Reactivate the menu now that we are done
    NGenFunc::DispatchEvent( NMenuDefs::EME_MENU_REACTIVATE );
}
