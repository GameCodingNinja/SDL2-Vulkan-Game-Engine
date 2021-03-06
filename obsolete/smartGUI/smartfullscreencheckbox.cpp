
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
*    DESC:  Constructor
************************************************************************/
CSmartScrnCheckBox::CSmartScrnCheckBox( iControl * piControl ) :
    CSmartSettingsMenuBtn( piControl )
{
}


/***************************************************************************
*    DESC:  Handle events
****************************************************************************/
void CSmartScrnCheckBox::handleEvent( const SDL_Event & rEvent )
{
    if( rEvent.type == NMenuDefs::EME_MENU_TRANS_IN )
        m_piControl->setToggleState( CSettings::Instance().getFullScreen() );
}


/***************************************************************************
*    DESC:  Called when the control is executed - Enable/disable the apply btn
****************************************************************************/
void CSmartScrnCheckBox::execute()
{
    enableDisableApplyBtn();
}
