/************************************************************************
*    FILE NAME:       uilabel.cpp
*
*    DESCRIPTION:     Class for user interface labels
************************************************************************/

// Physical component dependency
#include <gui/uilabel.h>

/************************************************************************
*    DESC:  Constructor                                                             
************************************************************************/
CUILabel::CUILabel( const std::string & group ) :
    CUIControl( group )
{
    m_type = EControlType::LABEL;

}   // Constructor

/************************************************************************
*    DESC:  Destructor                                                             
************************************************************************/
CUILabel::~CUILabel()
{
}

