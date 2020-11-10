
/************************************************************************
*    FILE NAME:       fontdata.cpp
*
*    DESCRIPTION:     font data class
************************************************************************/

// Physical component dependency
#include <common/fontdata.h>

// Game lib dependencies
#include <utilities/xmlParser.h>

/************************************************************************
*    DESC:  Copy the data
************************************************************************/ 
void CFontData::copy( const CFontData & obj )
{
    m_fontString = obj.m_fontString;
    m_fontStrSize = obj.m_fontStrSize;
    m_fontProp.copy( obj.m_fontProp );
}

/************************************************************************
*    DESC:  Load the font properties from XML node
************************************************************************/ 
void CFontData::loadFromNode( const XMLNode & node )
{
    const XMLNode fontNode = node.getChildNode( "font" );
    if( !fontNode.isEmpty() )
    {
        // See if a font string has been defined
        if( fontNode.isAttributeSet("fontString") )
            m_fontString = fontNode.getAttribute( "fontString" );

        m_fontProp.loadFromNode( fontNode );
    }
}
