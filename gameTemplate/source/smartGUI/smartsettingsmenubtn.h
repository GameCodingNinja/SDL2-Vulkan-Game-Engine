
/************************************************************************
*    FILE NAME:       smartsettingsmenubtn.h
*
*    DESCRIPTION:     Class CSmartButton
************************************************************************/

#ifndef __smart_btn_h__
#define __smart_btn_h__

// Physical component dependency
#include <gui/ismartguibase.h>

class CSmartSettingsMenuBtn : public CSmartGuiControl
{
public:

    // Constructor
    CSmartSettingsMenuBtn( CUIControl * pUIControl );

protected:

    // Enable/disable the apply btn
    void enableDisableApplyBtn();

    // Were the buttons in the settings menu changed
    bool wasSettingsButtonsChanged();
};

#endif  // __smart_btn_h__


