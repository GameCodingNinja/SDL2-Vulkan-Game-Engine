
/************************************************************************
*    FILE NAME:       smartdeadzoneslider.h
*
*    DESCRIPTION:     Class CSmartVSyncCheckBox
************************************************************************/

#ifndef __smart_dead_zone_slider_h__
#define __smart_dead_zone_slider_h__

// Physical component dependency
#include "smartsettingsmenubtn.h"

class CSmartDeadZoneSlider : public CSmartSettingsMenuBtn
{
public:

    // Constructor
    CSmartDeadZoneSlider( iControl * piControl );
    
    // Handle events
    void handleEvent( const SDL_Event & rEvent ) override;

    // Called when the control is executed
    void execute() override;
};

#endif
