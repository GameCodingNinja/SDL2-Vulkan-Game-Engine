/************************************************************************
*    FILE NAME:       uilabel.h
*
*    DESCRIPTION:     Class for user interface labels
************************************************************************/

#pragma once

// Physical component dependency
#include <gui/uicontrol.h>

class CUILabel : public CUIControl
{
public:
    
    // Constructor
    CUILabel( const std::string & group );

    // Destructor
    virtual ~CUILabel();
};
