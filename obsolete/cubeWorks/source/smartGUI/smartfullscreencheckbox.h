
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
    CSmartScrnCheckBox( CUIControl * pUIControl );

    // Handle events
    void HandleEvent( const SDL_Event & rEvent );

    // Called when the control is executed
    void Execute();

};

#endif  // __smart_full_screen_check_box_h__


