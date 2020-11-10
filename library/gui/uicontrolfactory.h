/************************************************************************
*    FILE NAME:       uicontrolfactory.h
*
*    DESCRIPTION:     Class factory for control creation
************************************************************************/

#pragma once

// Standard lib dependencies
#include <string>

// Forward declaration(s)
class iControl;
struct XMLNode;

namespace NUIControlFactory
{
    // Create the control
    iControl * Create( const XMLNode & node, const std::string & group );
};

