
/************************************************************************
*    FILE NAME:       smartdeadzoneslider.cpp
*
*    DESCRIPTION:     Class CSmartVSyncCheckBox
************************************************************************/

// Physical component dependency
#include "smartdeadzoneslider.h"

// Game lib dependencies
#include <gui/uislider.h>
#include <utilities/genfunc.h>
#include <utilities/settings.h>

/************************************************************************
*    desc:  Constructer
************************************************************************/
CSmartDeadZoneSlider::CSmartDeadZoneSlider( CUIControl * pUIControl ) :
    CSmartSettingsMenuBtn( pUIControl )
{
}   // constructor


/***************************************************************************
*    decs:  Handle events
****************************************************************************/
void CSmartDeadZoneSlider::HandleEvent( const SDL_Event & rEvent )
{
    if( rEvent.type == NMenu::EGE_MENU_TRANS_IN )
        (*dynamic_cast<CUISlider *>(m_pUIControl)).SetSlider( CSettings::Instance().GetGamePadStickDeadZone() );

}   // HandleEvent


/***************************************************************************
*    decs:  Called when the control is executed - Enable/disable the apply btn
****************************************************************************/
void CSmartDeadZoneSlider::Execute()
{
    EnableDisableApplyBtn();

}   // Execute







