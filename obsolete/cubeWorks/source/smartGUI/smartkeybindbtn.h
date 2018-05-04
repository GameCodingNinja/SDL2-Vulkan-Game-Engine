
/************************************************************************
*    FILE NAME:       smartkeybindbtn.h
*
*    DESCRIPTION:     Class CSmartKeyBindBtn
************************************************************************/

#ifndef __smart_key_bind_btn_h__
#define __smart_key_bind_btn_h__

// Physical component dependency
#include <gui/ismartguibase.h>

class CSmartKeyBindBtn : public CSmartGuiControl
{
public:

    // Constructor
    CSmartKeyBindBtn( CUIControl * pUIControl );

    // Called when the control is created
    // Sets the action ID string for the given device
    void Create();
    
    // Called when the control is executed
    void Execute();
    
    // Handle events
    void HandleEvent( const SDL_Event & rEvent );

};

#endif  // __smart_key_bind_btn_h__


