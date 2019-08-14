
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
    CSmartKeyBindBtn( iControl * piControl );

    // Called when the control is created
    // Sets the action ID string for the given device
    void create() override;
    
    // Called when the control is executed
    void execute() override;
    
    // Handle events
    void handleEvent( const SDL_Event & rEvent ) override;
};

#endif
