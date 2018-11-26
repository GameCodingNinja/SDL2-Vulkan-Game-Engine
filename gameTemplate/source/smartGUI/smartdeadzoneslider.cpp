
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
*    DESC:  Constructor
************************************************************************/
CSmartDeadZoneSlider::CSmartDeadZoneSlider( iControl * piControl ) :
    CSmartSettingsMenuBtn( piControl )
{
}


/***************************************************************************
*    DESC:  Handle events
****************************************************************************/
void CSmartDeadZoneSlider::handleEvent( const SDL_Event & rEvent )
{
    if( rEvent.type == NMenuDefs::EME_MENU_TRANS_IN )
        m_piControl->setSlider( CSettings::Instance().getGamePadStickDeadZone() );
}


/***************************************************************************
*    DESC:  Called when the control is executed - Enable/disable the apply btn
****************************************************************************/
void CSmartDeadZoneSlider::execute()
{
    enableDisableApplyBtn();
}
