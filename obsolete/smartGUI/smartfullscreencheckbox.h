
/************************************************************************
*    FILE NAME:       smartfullscreencheckbox.h
*
*    DESCRIPTION:     Class CSmartScrnCheckBox
************************************************************************/

#ifndef __smart_full_screen_check_box_h__
#define __smart_full_screen_check_box_h__

// Physical component dependency
#include "smartsettingsmenubtn.h"

// Forward declaration(s)
class CUIControl;

class CSmartScrnCheckBox : public CSmartSettingsMenuBtn
{
public:

    // Constructor
    CSmartScrnCheckBox( iControl * piControl );

    // Handle events
    void handleEvent( const SDL_Event & rEvent ) override;

    // Called when the control is executed
    void execute() override;
};

#endif
