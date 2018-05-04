
/************************************************************************
*    FILE NAME:       smartconfirmbtn.h
*
*    DESCRIPTION:     Class CSmartConfirmBtn
************************************************************************/

#ifndef __smart_confirmation_btn_h__
#define __smart_confirmation_btn_h__

// Physical component dependency
#include <gui/ismartguibase.h>

class CSmartConfirmBtn : public CSmartGuiControl
{
public:

    // Constructor
    CSmartConfirmBtn( CUIControl * pUIControl );

    // Called when the control is executed
    void Execute();

};

#endif  // __smart_exit_btn_h__


