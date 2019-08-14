
/************************************************************************
*    FILE NAME:       smartsettingsmenubtn.cpp
*
*    DESCRIPTION:     Class CSmartExitBtn
************************************************************************/

// Physical component dependency
#include "smartsettingsmenubtn.h"

// Game dependencies
#include "smartresolutionbtn.h"

// Game lib dependencies
#include <gui/menumanager.h>
#include <gui/icontrol.h>
#include <gui/uicheckbox.h>
#include <gui/uibuttonlist.h>
#include <gui/menu.h>
#include <gui/uislider.h>
#include <utilities/genfunc.h>
#include <utilities/settings.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSmartSettingsMenuBtn::CSmartSettingsMenuBtn( iControl * piControl ) :
    CSmartGuiControl( piControl )
{
}


/***************************************************************************
*    DESC:  Enable/disable the apply btn
****************************************************************************/
void CSmartSettingsMenuBtn::enableDisableApplyBtn()
{
    // Get the settings menu
    CMenu & rMenu = CMenuMgr::Instance().getActiveMenu();

    if( wasSettingsButtonsChanged() )
        rMenu.getPtrToControl( "settings_apply_btn" )->changeState(NUIControlDefs::ECS_INACTIVE);
    else
        rMenu.getPtrToControl( "settings_apply_btn" )->changeState(NUIControlDefs::ECS_DISABLE);
}


/***************************************************************************
*    DESC:  Were the buttons in the settings menu changed
****************************************************************************/
bool CSmartSettingsMenuBtn::wasSettingsButtonsChanged()
{
    // Get the settings menu
    CMenu & rMenu = CMenuMgr::Instance().getActiveMenu();

    // Get the control
    iControl * pControl;

    // Check for the resolution button list
    pControl = rMenu.getPtrToControl( "resolution_btn_lst" );
    if( (*dynamic_cast<CSmartResolutionBtn *>(pControl->getSmartGuiPtr())).wasResolutionChanged() )
        return true;

    // Check for the full screen check box
    pControl = rMenu.getPtrToControl( "full_screen_check_box" );
    if( CSettings::Instance().getFullScreen() != pControl->getToggleState() )
        return true;

    // Check for the v-sync check box
    pControl = rMenu.getPtrToControl( "v-sync_check_box" );
    if( CSettings::Instance().getVSync() != pControl->getToggleState() )
        return true;

    // Check for dead zone slider
    pControl = rMenu.getPtrToControl( "settings_dead_zone_slider" );
    if( CSettings::Instance().getGamePadStickDeadZone() != (int)pControl->getSliderPosValue() )
        return true;

    return false;
}
