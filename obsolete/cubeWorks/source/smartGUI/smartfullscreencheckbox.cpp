
/************************************************************************
*    FILE NAME:       smartfullscreencheckbox.cpp
*
*    DESCRIPTION:     Class CSmartScrnCheckBox
************************************************************************/

// Physical component dependency
#include "smartfullscreencheckbox.h"

// Game lib dependencies
#include <gui/uicheckbox.h>
#include <utilities/settings.h>
#include <utilities/genfunc.h>

/************************************************************************
*    desc:  Constructer
************************************************************************/
CSmartScrnCheckBox::CSmartScrnCheckBox( CUIControl * pUIControl ) :
    CSmartSettingsMenuBtn( pUIControl )
{
}   // constructor


/***************************************************************************
*    decs:  Handle events
****************************************************************************/
void CSmartScrnCheckBox::HandleEvent( const SDL_Event & rEvent )
{
    if( rEvent.type == NMenu::EGE_MENU_TRANS_IN )
        (*dynamic_cast<CUICheckBox *>(m_pUIControl)).SetToggleState( CSettings::Instance().GetFullScreen() );

}   // HandleEvent


/***************************************************************************
*    decs:  Called when the control is executed - Enable/disable the apply btn
****************************************************************************/
void CSmartScrnCheckBox::Execute()
{
    EnableDisableApplyBtn();

}   // Execute







