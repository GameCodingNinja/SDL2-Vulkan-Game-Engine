/************************************************************************
*    FILE NAME:       uibutton.h
*
*    DESCRIPTION:     Class for user interface buttons
************************************************************************/

#pragma once

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
