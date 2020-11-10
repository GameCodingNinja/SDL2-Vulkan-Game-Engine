/************************************************************************
*    FILE NAME:       fontdata.h
*
*    DESCRIPTION:     fontdata class
************************************************************************/  

#pragma once

// Game lib dependencies
#include <common/fontproperties.h>
#include <common/size.h>

// Forward Declarations
struct XMLNode;

class CFontData
{
public:
    
    // Copy the data
    void copy( const CFontData & obj );
    
    // Load the font properties from XML node
    void loadFromNode( const XMLNode & node );
    
public:
    
    // Displayed font string
    std::string m_fontString;

    // Font members
    CFontProperties m_fontProp;

    // Font string size
    // Not use full for multi-line strings
    CSize<float> m_fontStrSize;
};
