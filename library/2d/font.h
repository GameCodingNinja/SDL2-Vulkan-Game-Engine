/************************************************************************
*    FILE NAME:       font.h
*
*    DESCRIPTION:     Font class
************************************************************************/  

#pragma once

// Game lib dependencies
#include <common/size.h>
#include <common/rect.h>
#include <common/texture.h>

// Standard lib dependencies
#include <string>
#include <map>

class CCharData
{
public:
    CCharData() : xAdvance(0) {}
    ~CCharData() {}

    // Character offsets
    CSize<float> offset;

    // Character rect
    CRect<float> rect;

    // Amount to advance
    float xAdvance;
};

class CFont
{
public:

    CFont( const std::string & filePath );
    ~CFont();

    // Load from XML file
    void load( const std::string & group );

    // Get the data for this character
    const CCharData & getCharData( char id ) const;

    // Get the line height
    float getLineHeight() const;

    // Get the baseline offset
    float getBaselineOffset() const;

    // Get the horzontal padding
    float getHorzPadding() const;

    // Get the vertical padding
    float getVertPadding() const;

    // Get the texture size
    const CSize<int> & getTextureSize() const;

    // Get the texture ID
    uint32_t getTextureID() const;
    
    // Get the texture
    const CTexture & getTexture() const;
    
private:

    // font file path
    std::string m_filePath;
    
    // map list of character data
    std::map<char, CCharData> m_charDataMap;

    // Line height
    float m_lineHeight;

    // base line offset
    float m_baselineOffset;

    // Horizontal padding
    float m_horzPadding;

    // Vertival padding
    float m_vertPadding;

    // The texture
    CTexture m_texture;
};
