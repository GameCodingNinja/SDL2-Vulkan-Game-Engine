/************************************************************************
*    FILE NAME:       uicontrolfactory.h
*
*    DESCRIPTION:     Class factory for control creation
************************************************************************/

#ifndef __ui_control_factory_h__
#define __ui_control_factory_h__

// Standard lib dependencies
#include <string>

// Forward declaration(s)
class CUIControl;
struct XMLNode;

namespace NUIControlFactory
{
    // Create the control
    CUIControl * Create( const XMLNode & node, const std::string & group );
};

#endif
