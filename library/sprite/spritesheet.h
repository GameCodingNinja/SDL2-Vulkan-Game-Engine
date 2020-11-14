
/************************************************************************
*    FILE NAME:       spritesheet.h
*
*    DESCRIPTION:     Class for holding sprite sheet data
************************************************************************/

#pragma once

// Game lib dependencies
#include <sprite/spritesheetglyph.h>

// Standard lib dependencies
#include <string>
#include <vector>
#include <map>

class CSpriteSheet
{
public:
    
    // Constructor
    CSpriteSheet();
    
    // Set the glyph count
    void setGlyphCount( uint glyphCount);
    
    // Set the glyph columns
    void setGlyphColumns( uint columns );
    
    // Build the simple (grid) sprite sheet
    void build( const CSize<int> & sheetSize );
    
    // Load the glyph data
    void load( const std::string & filePath );
    
    // Get the glyph
    const CSpriteSheetGlyph & getGlyph( int index = -1 ) const;
    const CSpriteSheetGlyph & findGlyph( const std::string & glyphId ) const;
   
    // Get the number of gylphs in this sprite sheet
    size_t getCount() const;
    
    // Set/Get the default index
    void setDefaultIndex( uint index );
    uint getDefaultIndex() const;
    
    // Set/Get the start index for when loading from format code
    void setFormatCodeOffset( uint index );
    int getFormatCodeOffset() const;
    
    // Copy over the gylph data
    void copyTo( CSpriteSheet & rSpriteSheet, const std::vector<std::string> & strIdVec, bool loadAllGlyphs = false ) const;
    
    // Clear out the data
    void clear();
    
private:

    // Set the gylph data
    void set( const CSpriteSheetGlyph & rGlyph );
    void set( CSpriteSheet & rSpriteSheet, const std::string & rGlyphId ) const;
    void set( const CSpriteSheet & rSpriteSheet, const std::vector<std::string> & rStrIdVec );
    
private:

    // Sprite Sheet default index
    uint m_defaultIndex;
    
    // Sprite Sheet element count
    uint m_glyphCount;

    // Sprite Sheet columns
    uint m_columns;
    
    // Starting offset for loading with a format code
    int m_formatCodeOffset;
    
    // Size of the sprite sheet
    CSize<int> m_size;

    // A vector of all the glyphs built manually that are of the same size
    std::vector< CSpriteSheetGlyph > m_glyphVec;
    
    // A map of all the glyphs
    std::map< std::string, CSpriteSheetGlyph > m_glyphMap;
};
