
/************************************************************************
*    FILE NAME:       smartapplysettingsbtn.h
*
*    DESCRIPTION:     Class CSmartApplySettingsBtn
************************************************************************/

#ifndef __smart_apply_settings_btn_h__
#define __smart_apply_settings_btn_h__

// Physical component dependency
#include "smartsettingsmenubtn.h"

class CSmartApplySettingsBtn : public CSmartSettingsMenuBtn
{
public:

    // Constructor
    CSmartApplySettingsBtn( CUIControl * pUIControl );
    
    // Handle events
    void handleEvent( const SDL_Event & rEvent ) override;

    // Called when the control is to excute
    void execute() override;
};

#endif  // __smart_apply_settings_btn_h__


