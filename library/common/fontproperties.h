/************************************************************************
*    FILE NAME:       fontproperties.h
*
*    DESCRIPTION:     font properties class
************************************************************************/  

#ifndef __font_properties_h__
#define __font_properties_h__

// Game lib dependencies
#include <common/defs.h>

// Standard lib dependencies
#include <string>

// Forward Declarations
struct XMLNode;

class CFontProperties
{
public:
    
    CFontProperties();
    CFontProperties( const std::string fontName );
    CFontProperties( const std::string fontName, NDefs::EHorzAlignment hAlign, NDefs::EVertAlignment vAlign );
    CFontProperties( const std::string fontName, NDefs::EHorzAlignment hAlign, NDefs::EVertAlignment vAlign, float kerning );
    
    // Copy the data
    void copy( const CFontProperties & obj );
    
    // oad the font properties from XML node
    void loadFromNode( const XMLNode & node );
    
public:

    // font to use
    std::string m_fontName;
    
    // horzontal alignment
    NDefs::EHorzAlignment m_hAlign = NDefs::EHA_HORZ_CENTER;
    
    // vertical alignment
    NDefs::EVertAlignment m_vAlign = NDefs::EVA_VERT_CENTER;
    
    // distance between each character
    float m_kerning = 0.f;
    
    // special kerning just for the space character
    float m_spaceCharKerning = 0.f;
    
    // width of line to force wrap
    float m_lineWrapWidth = -1.f;
    
    // add spacing to the lines
    float m_lineWrapHeight = 0.f;
};

#endif  // __font_properties_h__

