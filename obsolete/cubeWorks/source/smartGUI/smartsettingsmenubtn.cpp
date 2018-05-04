
/************************************************************************
*    FILE NAME:       smartsettingsmenubtn.cpp
*
*    DESCRIPTION:     Class CSmartExitBtn
************************************************************************/

// Physical component dependency
#include "smartsettingsmenubtn.h"

// Game lib dependencies
#include <gui/menumanager.h>
#include <gui/uicontrol.h>
#include <gui/uicheckbox.h>
#include <gui/uibuttonlist.h>
#include <gui/menu.h>
#include <gui/uislider.h>
#include <utilities/genfunc.h>
#include <utilities/settings.h>

// Game dependencies
#include "smartresolutionbtn.h"

/************************************************************************
*    desc:  Constructer
************************************************************************/
CSmartSettingsMenuBtn::CSmartSettingsMenuBtn( CUIControl * pUIControl ) :
    CSmartGuiControl( pUIControl )
{
}   // constructor


/***************************************************************************
*    decs:  Enable/disable the apply btn
****************************************************************************/
void CSmartSettingsMenuBtn::EnableDisableApplyBtn()
{
    // Get the settings menu
    CMenu & rMenu = CMenuManager::Instance().GetMenu( "settings_menu" );

    if( WasSettingsButtonsChanged() )
        rMenu.GetPtrToControl( "settings_apply_btn" )->ChangeState(NUIControl::ECS_INACTIVE);
    else
        rMenu.GetPtrToControl( "settings_apply_btn" )->ChangeState(NUIControl::ECS_DISABLED);

}   // EnableDisableSettingsApplyBtn


/***************************************************************************
*    decs:  Were the buttons in the settings menu changed
****************************************************************************/
bool CSmartSettingsMenuBtn::WasSettingsButtonsChanged()
{
    // Get the settings menu
    CMenu & rMenu = CMenuManager::Instance().GetMenu( "settings_menu" );

    // Get the control
    CUIControl * pControl;

    // Check for the resolution button list
    pControl = rMenu.GetPtrToControl( "resolution_btn_lst" );
    if( (*dynamic_cast<CSmartResolutionBtn *>(pControl->GetSmartGuiPtr())).WasResolutionChanged() )
        return true;

    // Check for the full screen check box
    pControl = rMenu.GetPtrToControl( "full_screen_check_box" );
    if( CSettings::Instance().GetFullScreen() != (*dynamic_cast<CUICheckBox *>(pControl)).GetToggleState() )
        return true;

    // Check for the v-sync check box
    pControl = rMenu.GetPtrToControl( "v-sync_check_box" );
    if( CSettings::Instance().GetVSync() != (*dynamic_cast<CUICheckBox *>(pControl)).GetToggleState() )
        return true;

    // Check for dead zone slider
    pControl = rMenu.GetPtrToControl( "settings_dead_zone_slider" );
    if( CSettings::Instance().GetGamePadStickDeadZone() != (int)(*dynamic_cast<CUISlider *>(pControl)).GetValue() )
        return true;

    return false;

}   // WasSettingsChanged







