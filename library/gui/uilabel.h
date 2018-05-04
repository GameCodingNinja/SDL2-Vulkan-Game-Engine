/************************************************************************
*    FILE NAME:       uilabel.h
*
*    DESCRIPTION:     Class for user interface labels
************************************************************************/

#ifndef __ui_label_h__
#define __ui_label_h__

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

#endif  // __ui_label_h__