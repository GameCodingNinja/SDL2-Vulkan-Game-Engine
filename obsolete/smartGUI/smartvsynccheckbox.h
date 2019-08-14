
/************************************************************************
*    FILE NAME:       smartvsynccheckbox.h
*
*    DESCRIPTION:     Class CSmartVSyncCheckBox
************************************************************************/

#ifndef __smart_vsync_check_box_h__
#define __smart_vsync_check_box_h__

// Physical component dependency
#include "smartsettingsmenubtn.h"

class CSmartVSyncCheckBox : public CSmartSettingsMenuBtn
{
public:

    // Constructor
    CSmartVSyncCheckBox( iControl * piControl );

    // Handle events
    void handleEvent( const SDL_Event & rEvent ) override;

    // Called when the control is executed
    void execute() override;
};

#endif
