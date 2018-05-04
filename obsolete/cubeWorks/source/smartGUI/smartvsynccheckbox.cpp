
/************************************************************************
*    FILE NAME:       smartvsynccheckbox.cpp
*
*    DESCRIPTION:     Class CSmartVSyncCheckBox
************************************************************************/

// Physical component dependency
#include "smartvsynccheckbox.h"

// Game lib dependencies
#include <gui/uicheckbox.h>
#include <utilities/settings.h>
#include <utilities/genfunc.h>

/************************************************************************
*    desc:  Constructer
************************************************************************/
CSmartVSyncCheckBox::CSmartVSyncCheckBox( CUIControl * pUIControl ) :
    CSmartSettingsMenuBtn( pUIControl )
{
}   // constructor


/***************************************************************************
*    decs:  Handle events
****************************************************************************/
void CSmartVSyncCheckBox::HandleEvent( const SDL_Event & rEvent )
{
    if( rEvent.type == NMenu::EGE_MENU_TRANS_IN )
        (*dynamic_cast<CUICheckBox *>(m_pUIControl)).SetToggleState( CSettings::Instance().GetVSync() );

}   // HandleEvent


/***************************************************************************
*    decs:  Called when the control is executed - Enable/disable the apply btn
****************************************************************************/
void CSmartVSyncCheckBox::Execute()
{
    EnableDisableApplyBtn();

}   // Execute

