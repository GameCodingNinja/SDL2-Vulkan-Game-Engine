
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
    CSmartVSyncCheckBox( CUIControl * pUIControl );

    // Handle events
    void HandleEvent( const SDL_Event & rEvent );

    // Called when the control is executed
    void Execute();

};

#endif  // __smart_vsync_check_box_h__


