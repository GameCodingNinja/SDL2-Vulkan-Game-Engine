
/************************************************************************
*    FILE NAME:       smartapplysettingsbtn.cpp
*
*    DESCRIPTION:     Class CSmartExitBtn
************************************************************************/

// Physical component dependency
#include "smartapplysettingsbtn.h"

#if defined(__IPHONEOS__) || defined(__ANDROID__)
#include "SDL_opengles2.h"
#else
#include <SDL_opengl.h>  // SDL/OpenGL lib dependencies
#endif

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <system/device.h>
#include <gui/menumanager.h>
#include <gui/uicheckbox.h>
#include <gui/uibuttonlist.h>
#include <gui/menu.h>
#include <gui/uislider.h>
#include <common/defs.h>
//#include <managers/hudmanager.h>
#include <utilities/genfunc.h>
#include <utilities/settings.h>

// Game dependencies
#include "../game/game.h"

// Game dependencies local
#include "smartresolutionbtn.h"

/************************************************************************
*    desc:  Constructer
************************************************************************/
CSmartApplySettingsBtn::CSmartApplySettingsBtn( CUIControl * pUIControl ) :
    CSmartSettingsMenuBtn( pUIControl )
{
}   // constructor


/***************************************************************************
*    decs:  Handle events
****************************************************************************/
void CSmartApplySettingsBtn::HandleEvent( const SDL_Event & rEvent )
{
    if( rEvent.type == NMenu::EGE_MENU_TRANS_IN )
        m_pUIControl->ChangeState(NUIControl::ECS_DISABLED);

}   // HandleEvent


/***************************************************************************
*    decs:  Called when the control is executed - Applies the settings
****************************************************************************/
void CSmartApplySettingsBtn::Execute()
{
    bool resChanged(false), fullScreenChanged(false);
    bool settingsChangeMade(false);

    // Get the settings menu
    CMenu & rMenu = CMenuManager::Instance().GetMenu( "settings_menu" );

    // Get the control
    CUIControl * pControl;

    // Check for the resolution button list
    CSmartResolutionBtn & rSmartRes = (*dynamic_cast<CSmartResolutionBtn *>(rMenu.GetPtrToControl( "resolution_btn_lst" )->GetSmartGuiPtr()));
    if( rSmartRes.WasResolutionChanged() )
    {
        rSmartRes.SetResolutionChange();
        resChanged = true;
        settingsChangeMade = true;
    }

    // Check for the full screen check box
    pControl = rMenu.GetPtrToControl( "full_screen_check_box" );
    const bool fullScreenState = (*dynamic_cast<CUICheckBox *>(pControl)).GetToggleState();
    const bool oldFullScreenState = CSettings::Instance().GetFullScreen();
    if( CSettings::Instance().GetFullScreen() != fullScreenState )
    {
        CSettings::Instance().SetFullScreen( fullScreenState );
        fullScreenChanged = true;
        settingsChangeMade = true;
    }

    // Check for the v-sync check box
    pControl = rMenu.GetPtrToControl( "v-sync_check_box" );
    const bool vsyncState = (*dynamic_cast<CUICheckBox *>(pControl)).GetToggleState();
    if( CSettings::Instance().GetVSync() != vsyncState )
    {
        CSettings::Instance().SetVSync( vsyncState );
        CDevice::Instance().EnableVSync( vsyncState );
        settingsChangeMade = true;
    }

    // Check for dead zone slider
    pControl = rMenu.GetPtrToControl( "settings_dead_zone_slider" );
    int sliderValue = (*dynamic_cast<CUISlider *>(pControl)).GetValue();
    if( CSettings::Instance().GetGamePadStickDeadZone() != sliderValue )
    {
        CSettings::Instance().SetGamePadStickDeadZone( sliderValue );
        settingsChangeMade = true;
    }

    if( resChanged )
    {
        // SDL2 doesn't allow res change in full screen
        // so take us out of full screen mode for res changes
        if( oldFullScreenState && fullScreenState )
            CDevice::Instance().SetFullScreen( false );

        SDL_DisplayMode mode;
        SDL_GetCurrentDisplayMode(0, &mode);

        mode.w = CSettings::Instance().GetSize().GetW();
        mode.h = CSettings::Instance().GetSize().GetH();
        mode.refresh_rate = 0;

        SDL_SetWindowDisplayMode( CDevice::Instance().GetWindow(), &mode );

        if( fullScreenChanged || fullScreenState )
            CDevice::Instance().SetFullScreen( fullScreenState );

        SDL_SetWindowSize(
            CDevice::Instance().GetWindow(),
            CSettings::Instance().GetSize().GetW(),
            CSettings::Instance().GetSize().GetH() );

        SDL_SetWindowPosition(
            CDevice::Instance().GetWindow(),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED );

        // Reset the world transform to recalculate mouse collision
        CMenuManager::Instance().ResetWorldTransform();

        // Reset the dynamic positions of menus
        CMenuManager::Instance().ResetDynamicOffset();

        // Need to reset the view port the changing the resolution
        glViewport(0, 0, CSettings::Instance().GetSize().GetW(), CSettings::Instance().GetSize().GetH());
    }
    else if( fullScreenChanged )
    {
        CDevice::Instance().SetFullScreen( fullScreenState );
    }

    // Only save if a change was actually made
    if( settingsChangeMade )
        CSettings::Instance().SaveSettings();

    m_pUIControl->ChangeState(NUIControl::ECS_DISABLED);

    // Reactivate the menu now that we are done
    NGenFunc::DispatchEvent( NMenu::EGE_MENU_REACTIVATE );

}   // Execute

