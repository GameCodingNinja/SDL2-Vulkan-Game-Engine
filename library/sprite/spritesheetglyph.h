
/************************************************************************
*    FILE NAME:       spritesheetglyph.h
*
*    DESCRIPTION:     Class to hold sprite sheet glyph data
************************************************************************/

#pragma once

// Game lib dependencies
#include <common/size.h>
#include <common/rect.h>

class CSpriteSheetGlyph
{
public:
    
    CSpriteSheetGlyph( const CSize<int> & size, const CRect<float> & uv, const CSize<int> & cropOffset ) :
        m_size( size ),
        m_uv( uv ),
        m_cropOffset( cropOffset )
    {}
        
    CSpriteSheetGlyph( const CSize<int> & size, const CRect<float> & uv ) :
        m_size( size ),
        m_uv( uv )
    {}
    
    CSpriteSheetGlyph( const CSpriteSheetGlyph & glyph ) :
        m_size( glyph.m_size ),
        m_uv( glyph.m_uv ),
        m_cropOffset( glyph.m_cropOffset )
    {}
    
    // Get the size
    const CSize<int> & getSize() const
    { return m_size; }
    
    // Get the UV
    const CRect<float> & getUV() const
    { return m_uv; }
    
    // Get the crop offset
    const CSize<int> & getCropOffset() const
    { return m_cropOffset; }

private:
    
    // Size of the glyph
    CSize<int> m_size;
    
    // UV coordinates
    CRect<float> m_uv;
    
    // Crop Offset
    CSize<int> m_cropOffset;
};
