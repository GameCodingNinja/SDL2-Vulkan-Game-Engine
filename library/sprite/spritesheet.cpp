
/************************************************************************
*    FILE NAME:       spritesheet.cpp
*
*    DESCRIPTION:     Class for holding sprite sheet data
************************************************************************/

// Physical component dependency
#include <sprite/spritesheet.h>

// Game lib dependencies
#include <utilities/xmlParser.h>
#include <utilities/xmlparsehelper.h>
#include <utilities/exceptionhandling.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSpriteSheet::CSpriteSheet() :
    m_defaultIndex(0),
    m_glyphCount(0),
    m_columns(0),
    m_formatCodeOffset(0)
{
}


/************************************************************************
*    DESC:  Set/Get the glyph count
************************************************************************/
void CSpriteSheet::setGlyphCount( uint glyphCount)
{
    m_glyphCount = glyphCount;
}


/************************************************************************
*    DESC:  Set the glyph columns
************************************************************************/
void CSpriteSheet::setGlyphColumns( uint columns )
{
    m_columns = columns;
}


/************************************************************************
*    DESC:  Build the simple (grid) sprite sheet data
************************************************************************/
void CSpriteSheet::build( const CSize<int> & sheetSize )
{
    if( (m_glyphCount != 0) && (m_columns != 0) )
    {
        m_size = sheetSize;

        int rows = m_glyphCount / m_columns;

        if( (m_glyphCount % m_columns) > 0 )
            ++rows;

        // Reserve the number of glyphs
        m_glyphVec.reserve( m_glyphCount );

        // Calculate the size of the individual glyph. They are all the same size
        const CSize<float> size( sheetSize.w / m_columns, sheetSize.h / rows );

        for( int i = 0; i < rows; ++i )
        {
            for( uint j = 0; j < m_columns; ++j )
            {
                m_glyphVec.emplace_back(
                    size,
                    CRect<float>(
                        ((float)j * size.w) / (float)sheetSize.w,
                        ((float)i * size.h) / (float)sheetSize.h,
                        size.w / (float)sheetSize.w,
                        size.h / (float)sheetSize.h ) );

                // Break out after all the gylphs have been defined
                if( m_glyphVec.size() == (size_t)m_glyphCount )
                    break;
            }
        }
    }
}


/************************************************************************
*    DESC:  Load the glyph data from XML file
*
*           NOTE: The first time the sprite sheet is created, byStrID sets
*           the stage as to how it is stored. If false, it's all loaded
*           in the sprite sheets vector and if true, the map. It's assumed
*           that string Id's are for complex sprite sheets that are shared
*           among many sprites where as no string id is a simple sprite
*           sheet animation used by maybe one sprite.
************************************************************************/
void CSpriteSheet::load( const std::string & filePath )
{
    // Open and parse the XML file:
    const XMLNode node = XMLNode::openFileHelper( filePath.c_str(), "spriteSheet" );
    if( !node.isEmpty() )
    {
        m_size = NParseHelper::LoadSizeFromChild( node );

        for( int i = 0; i < node.nChildNode(); ++i )
        {
            const XMLNode rectNode = node.getChildNode(i);

            auto rect = NParseHelper::LoadRectFromChild( rectNode );

            const CSize<float> glyphSize( rect.x2, rect.y2 );

            const CRect<float> uv(
                (float)rect.x1 / (float)m_size.w,
                (float)rect.y1 / (float)m_size.h,
                (float)rect.x2 / (float)m_size.w,
                (float)rect.y2 / (float)m_size.h );

            const CSize<int> cropOffset(
                std::atoi( rectNode.getAttribute( "cx" ) ),
                std::atoi( rectNode.getAttribute( "cy" ) ) );

            // Add the gylph to the map
            std::string strId = rectNode.getAttribute( "name" );
            m_glyphMap.emplace( std::piecewise_construct, std::forward_as_tuple(strId), std::forward_as_tuple(glyphSize, uv, cropOffset) );
        }
    }
}


/************************************************************************
*    DESC:  Set the gylph data
************************************************************************/
void CSpriteSheet::set( const CSpriteSheetGlyph & rGlyph )
{
    m_glyphVec.push_back( rGlyph );
}

void CSpriteSheet::set( CSpriteSheet & rSpriteSheet, const std::string & rGlyphId ) const
{
    rSpriteSheet.set( findGlyph( rGlyphId ) );
}

void CSpriteSheet::set( const CSpriteSheet & rSpriteSheet, const std::vector<std::string> & rStrIdVec )
{
    // Copy over the map entries
    for( auto & iter: rStrIdVec )
    {
        auto & rGlyph = rSpriteSheet.findGlyph( iter );
        m_glyphMap.emplace( iter, rGlyph );
        m_glyphVec.push_back( rGlyph );
    }
}


/************************************************************************
*    DESC:  Copy over the gylph data
************************************************************************/
void CSpriteSheet::copyTo( CSpriteSheet & rSpriteSheet, const std::vector<std::string> & rStrIdVec, bool loadAllGlyphs ) const
{
    if( rStrIdVec.empty() )
    {
        if( loadAllGlyphs )
        {
            rSpriteSheet.clear();

            for( auto & iter : m_glyphMap )
                rSpriteSheet.set( iter.second );
        }
    }
    else if( !m_glyphMap.empty() )
    {
        // Init the sprite sheet class with an animation that is one glyph defined with a format code
        // Should only be one entry
        if ((rStrIdVec.size() == 1) && (rStrIdVec.back().find('%') != std::string::npos))
        {
            const int formatCodeOffset = rSpriteSheet.getFormatCodeOffset();
            for( uint i = 0; i < rSpriteSheet.getCount(); ++i )
            {
                std::string glyphStr = boost::str( boost::format(rStrIdVec.back()) % (formatCodeOffset+i) );
                set( rSpriteSheet, glyphStr );
            }
        }
        // Init the sprite sheet class when each glyph is defined in the object data
        else if( rSpriteSheet.getCount() == 0 )
        {
            rSpriteSheet.set( *this, rStrIdVec );
        }
        else
        {
            throw NExcept::CCriticalException("Sprite Sheet Error!",
                boost::str( boost::format("Incorrect configuration.\n\n%s\nLine: %s")
                    % __FUNCTION__ % __LINE__ ));
        }
    }
}


/************************************************************************
*    DESC:  Get the glyph
************************************************************************/
const CSpriteSheetGlyph & CSpriteSheet::getGlyph( int index ) const
{
    if( index > -1 )
        return m_glyphVec.at( index );

    else
        return m_glyphVec.at( m_defaultIndex );
}

const CSpriteSheetGlyph & CSpriteSheet::findGlyph( const std::string & glyphId ) const
{
    auto mapIter = m_glyphMap.find( glyphId );
    if( mapIter == m_glyphMap.end() )
    {
        throw NExcept::CCriticalException("Sprite Sheet Error!",
            boost::str( boost::format("Glyph name is missing (%s).\n\n%s\nLine: %s")
                % glyphId % __FUNCTION__ % __LINE__ ));
    }

    return mapIter->second;
}


/************************************************************************
*    DESC:  Get the number of gylphs in this sprite sheet
************************************************************************/
size_t CSpriteSheet::getCount() const
{
    size_t count = m_glyphMap.size();
    const size_t vecCount = m_glyphVec.size();

    if( count < vecCount )
        count = vecCount;

    if( count < m_glyphCount )
        return m_glyphCount;

    return count;
}


/************************************************************************
*    DESC:  Set/Get the default index
************************************************************************/
void CSpriteSheet::setDefaultIndex( uint index )
{
    m_defaultIndex = index;
}

uint CSpriteSheet::getDefaultIndex() const
{
    return m_defaultIndex;
}


/************************************************************************
*    DESC:  Set/Get the offset for loading with a format code
************************************************************************/
void CSpriteSheet::setFormatCodeOffset( uint index )
{
    m_formatCodeOffset = index;
}

int CSpriteSheet::getFormatCodeOffset() const
{
    return m_formatCodeOffset;
}


/************************************************************************
*    DESC:  Clear out the data
************************************************************************/
void CSpriteSheet::clear()
{
    m_glyphVec.clear();
    m_glyphMap.clear();
}