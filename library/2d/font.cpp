/************************************************************************
*    FILE NAME:       font.cpp
*
*    DESCRIPTION:     font class
************************************************************************/

// Physical component dependency
#include <2d/font.h>

// Game lib dependencies
#include <utilities/xmlParser.h>
#include <utilities/exceptionhandling.h>
#include <system/device.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CFont::CFont( const std::string & filePath )
    : m_filePath(filePath),
      m_lineHeight(0),
      m_baselineOffset(0),
      m_horzPadding(0),
      m_vertPadding(0)
{
}

/************************************************************************
*    DESC:  Destructor
************************************************************************/
CFont::~CFont()
{
}

/************************************************************************
*    DESC:  Load from XML file
************************************************************************/
void CFont::load( const std::string & group )
{
    // open this file and parse
    XMLNode mainNode = XMLNode::openFileHelper( (m_filePath + ".fnt").c_str(), "font" );

    // Get the padding
    std::string padding = mainNode.getChildNode( "info" ).getAttribute("padding");
    m_horzPadding = atof(padding.substr(6,1).c_str());
    m_vertPadding = atof(padding.substr(0,1).c_str());

    // Get the common font info
    XMLNode commonNode = mainNode.getChildNode( "common" );

    // get the line height
    m_lineHeight = atof(commonNode.getAttribute("lineHeight"));

    // get the baseline offset
    m_baselineOffset = atof(commonNode.getAttribute("base"));

    // Get the list of character info
    XMLNode charLstNode = mainNode.getChildNode( "chars" );

    // Load in the individual character data
    for( int i = 0; i < charLstNode.nChildNode(); ++i )
    {
        XMLNode charNode = charLstNode.getChildNode( "char", i );

        CCharData charData;

        // Get the offset of the character
        charData.offset.w = std::atof(charNode.getAttribute( "xoffset" ));
        charData.offset.h = std::atof(charNode.getAttribute( "yoffset" ));

        // Get the x advance of the character
        charData.xAdvance = std::atof(charNode.getAttribute( "xadvance" ));

        // Get the rect of the character
        charData.rect.x1 = std::atof(charNode.getAttribute( "x" ));
        charData.rect.y1 = std::atof(charNode.getAttribute( "y" ));
        charData.rect.x2 = std::atof(charNode.getAttribute( "width" ));
        charData.rect.y2 = std::atof(charNode.getAttribute( "height" ));

        // Get the character ID which is the ascii value of the character.
        char id = std::atoi(charNode.getAttribute( "id" ));

        // Add the character to our list
        m_charDataMap.emplace( id, charData );
    }
    
    m_texture.textFilePath = m_filePath + ".png";
    m_texture = CDevice::Instance().createTexture( group, m_texture );
}

/************************************************************************
*    DESC:  Get the data for this character
************************************************************************/
const CCharData & CFont::getCharData( char id ) const
{
    // See if this character is part of the map
    auto iter = m_charDataMap.find( id );

    if( iter == m_charDataMap.end() )
        throw NExcept::CCriticalException("Font character data Error!",
            boost::str( boost::format("Font character ID can't be found (%s).\n\n%s\nLine: %s")
                % id % __FUNCTION__ % __LINE__ ));

    return iter->second;
}

/************************************************************************
*    DESC:  Get the line height
************************************************************************/
float CFont::getLineHeight() const
{
    return m_lineHeight;
}

/************************************************************************
*    DESC:  Get the baseline offset
************************************************************************/
float CFont::getBaselineOffset() const
{
    return m_baselineOffset;
}

/************************************************************************
*    DESC:  Get the horzontal padding
************************************************************************/
float CFont::getHorzPadding() const
{
    return m_horzPadding;
}

/************************************************************************
*    DESC:  Get the vertical padding
************************************************************************/
float CFont::getVertPadding() const
{
    return m_vertPadding;
}

/************************************************************************
*    DESC:  Get the texture size
************************************************************************/
const CSize<int> & CFont::getTextureSize() const
{
    return m_texture.size;
}

/************************************************************************
*    DESC:  Get the texture ID
************************************************************************/
uint32_t CFont::getTextureID() const
{
    return 0;
}

/************************************************************************
*    DESC:  Get the texture
************************************************************************/
const CTexture & CFont::getTexture() const
{
    return m_texture;
}
