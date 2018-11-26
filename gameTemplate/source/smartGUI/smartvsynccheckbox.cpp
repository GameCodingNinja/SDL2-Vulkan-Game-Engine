
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
*    DESC:  Constructor
************************************************************************/
CSmartVSyncCheckBox::CSmartVSyncCheckBox( iControl * piControl ) :
    CSmartSettingsMenuBtn( piControl )
{
}


/***************************************************************************
*    DESC:  Handle events
****************************************************************************/
void CSmartVSyncCheckBox::handleEvent( const SDL_Event & rEvent )
{
    if( rEvent.type == NMenuDefs::EME_MENU_TRANS_IN )
        m_piControl->setToggleState( CSettings::Instance().getVSync() );
}


/***************************************************************************
*    DESC:  Called when the control is executed - Enable/disable the apply btn
****************************************************************************/
void CSmartVSyncCheckBox::execute()
{
    enableDisableApplyBtn();
}
