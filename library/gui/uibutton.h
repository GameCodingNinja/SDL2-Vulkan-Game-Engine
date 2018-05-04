/************************************************************************
*    FILE NAME:       uibutton.h
*
*    DESCRIPTION:     Class for user interface buttons
************************************************************************/

#ifndef __ui_button_h__
#define __ui_button_h__

// Physical component dependency
#include <gui/uicontrol.h>

class CUIButton : public CUIControl
{
public:
    
    // Constructor
    CUIButton( const std::string & group );

    // Destructor
    virtual ~CUIButton();

};

#endif  // __ui_label_h__