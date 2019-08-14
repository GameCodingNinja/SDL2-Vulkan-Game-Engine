
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
    CSmartConfirmBtn( iControl * piControl );

    // Called when the control is executed
    void execute() override;
};

#endif
