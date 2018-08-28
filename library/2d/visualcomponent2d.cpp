
/************************************************************************
*    FILE NAME:       visualcomponent2d.cpp
*
*    DESCRIPTION:     Class for handling the visual part of the sprite
************************************************************************/

// Physical component dependency
#include <2d/visualcomponent2d.h>

// Game lib dependencies
#include <objectdata/objectvisualdata2d.h>
#include <managers/shadermanager.h>
#include <managers/vertexbuffermanager.h>
#include <managers/fontmanager.h>
#include <common/quad2d.h>
#include <common/shaderdata.h>
#include <common/fontdata.h>
#include <system/device.h>
#include <utilities/xmlParser.h>
#include <utilities/xmlparsehelper.h>
#include <utilities/genfunc.h>
#include <utilities/exceptionhandling.h>
#include <utilities/statcounter.h>
#include <utilities/deletefuncs.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <memory>

#define GL_TRIANGLE_FAN 1
#define GL_TRIANGLES 1
#define GL_UNSIGNED_SHORT 1
#define GL_UNSIGNED_BYTE 1

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CVisualComponent2D::CVisualComponent2D( const CObjectVisualData2D & visualData ) :
    m_pShaderData(nullptr),
    //m_vbo( visualData.getVBO() ),
    //m_ibo( visualData.getIBO() ),
    m_textureID( visualData.getTextureID() ),
    m_vertexLocation(-1),
    m_uvLocation(-1),
    m_text0Location(-1),
    m_colorLocation(-1),
    m_matrixLocation(-1),
    m_glyphLocation(-1),
    GENERATION_TYPE( visualData.getGenerationType() ),
    m_quadVertScale( visualData.getVertexScale() ),
    m_rVisualData( visualData ),
    m_color( visualData.getColor() ),
    m_iboCount( visualData.getIBOCount() ),
    m_drawMode( (visualData.getGenerationType() == NDefs::EGT_QUAD || visualData.getGenerationType() == NDefs::EGT_SPRITE_SHEET) ? GL_TRIANGLE_FAN : GL_TRIANGLES ),
    m_indiceType( (visualData.getGenerationType() == NDefs::EGT_FONT) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_BYTE ),
    m_frameIndex(0),
    m_pFontData(nullptr)
{
    if( visualData.isActive() )
    {
        m_pShaderData = &CShaderMgr::Instance().getShaderData( visualData.getShaderID() );

        // Common shader members
        m_vertexLocation = m_pShaderData->getAttributeLocation( "in_position" );
        m_matrixLocation = m_pShaderData->getUniformLocation( "cameraViewProjMatrix" );
        m_colorLocation = m_pShaderData->getUniformLocation( "color" );

        // Do we have a texture? This could be a solid rect
        if( (m_textureID > 0) || (GENERATION_TYPE == NDefs::EGT_FONT) )
        {
            m_uvLocation = m_pShaderData->getAttributeLocation( "in_uv" );
            m_text0Location = m_pShaderData->getUniformLocation( "text0" );
        }

        // Is this a sprite sheet? Get the glyph rect position
        if( GENERATION_TYPE == NDefs::EGT_SPRITE_SHEET )
        {
            m_glyphLocation = m_pShaderData->getUniformLocation( "glyphRect" );

            m_glyphUV = visualData.getSpriteSheet().getGlyph().getUV();
            m_frameIndex = visualData.getSpriteSheet().getDefaultIndex();
        }

        // Allocate the storage for the font if this is a font sprite
        // Using a normal pointer to keep the memory foot print as small as possible
        if( GENERATION_TYPE == NDefs::EGT_FONT )
            m_pFontData = new CFontData;
    }
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CVisualComponent2D::~CVisualComponent2D()
{
    NDelFunc::Delete(m_pFontData);

    // Delete the VBO if this is a font
    deleteFontVBO();

    // The IBO for the font is managed by the vertex buffer manager.
    // Font IBO are all the same with the only difference being
    // length of the character string.
}


/************************************************************************
*    DESC:  Delete the custom VBO for this font
*
*           Added a VBO delete function so that deleting the VBO can be
*           handled separately so that the sprite can be allocated and
*           freed from a thread
************************************************************************/
void CVisualComponent2D::deleteFontVBO()
{
    // Delete the VBO if this is a font
    /*if( (GENERATION_TYPE == NDefs::EGT_FONT) && (m_vbo > 0) )
    {
        glDeleteBuffers(1, &m_vbo);
        m_vbo = 0;
    }*/

    // The IBO for the font is managed by the vertex buffer manager.
    // Font IBO are all the same with the only difference being
    // length of the character string.
}


/************************************************************************
*    DESC:  do the render
************************************************************************/
void CVisualComponent2D::render( const CMatrix & objMatrix, const CMatrix & matrix )
{
    /*if( allowRender() )
    {
        const int32_t VERTEX_BUF_SIZE( sizeof(CVertex2D) );

        // Increment our stat counter to keep track of what is going on.
        CStatCounter::Instance().incDisplayCounter();

        // Bind the VBO and IBO
        CVertBufMgr::Instance().bind( m_vbo, m_ibo );

        // Bind the shader. This must be done before glVertexAttribPointer
        CShaderMgr::Instance().bind( m_pShaderData );

        // Setup the vertex attribute shader data
        glVertexAttribPointer( m_vertexLocation, 3, GL_FLOAT, GL_FALSE, VERTEX_BUF_SIZE, (GLvoid*)0 );

        // Are we rendering with a texture?
        if( m_textureID > 0 )
        {
            const int8_t UV_OFFSET( sizeof(CPoint<float>) );

            // Bind the texture
            //CTextureMgr::Instance().bind( m_textureID );
            glUniform1i( m_text0Location, 0); // 0 = TEXTURE0

            // Setup the UV attribute shade data
            glVertexAttribPointer( m_uvLocation, 2, GL_FLOAT, GL_FALSE, VERTEX_BUF_SIZE, (GLvoid*)UV_OFFSET );
        }

        // Send the color to the shader
        glUniform4fv( m_colorLocation, 1, (float*)&m_color );

        // If this is a quad, we need to take into account the vertex scale
        if( GENERATION_TYPE == NDefs::EGT_QUAD )
        {
            // Calculate the final matrix
            CMatrix finalMatrix;
            finalMatrix.setScale( m_quadVertScale );
            finalMatrix *= objMatrix;
            finalMatrix *= matrix;

            // Send the final matrix to the shader
            glUniformMatrix4fv( m_matrixLocation, 1, GL_FALSE, finalMatrix() );
        }
        // If this is a sprite sheet, we need to take into account the vertex scale and glyph rect
        else if( GENERATION_TYPE == NDefs::EGT_SPRITE_SHEET )
        {
            // Calculate the final matrix
            CMatrix finalMatrix;
            finalMatrix.setScale( m_quadVertScale );
            finalMatrix *= objMatrix;
            finalMatrix *= matrix;

            // Send the final matrix to the shader
            glUniformMatrix4fv( m_matrixLocation, 1, GL_FALSE, finalMatrix() );

            // Send the glyph rect
            glUniform4fv( m_glyphLocation, 1, (float*)&m_glyphUV );
        }
        // this is for scaled frame and font rendering
        else
        {
            // Calculate the final matrix
            CMatrix finalMatrix;
            finalMatrix *= objMatrix;
            finalMatrix *= matrix;

            glUniformMatrix4fv( m_matrixLocation, 1, GL_FALSE, finalMatrix() );
        }

        // Render it
        glDrawElements( m_drawMode, m_iboCount, m_indiceType, nullptr );
    }*/
}


/************************************************************************
*    DESC:  Load the font properties from XML node
************************************************************************/
void CVisualComponent2D::loadFontPropFromNode( const XMLNode & node )
{
    if(m_pFontData)
        m_pFontData->loadFromNode( node );
}


/************************************************************************
*    DESC:  Set the font data
************************************************************************/
void CVisualComponent2D::setFontData( const CFontData & fontData )
{
    if(m_pFontData)
        m_pFontData->copy( fontData );
}


/************************************************************************
*    DESC:  Set the font properties
************************************************************************/
void CVisualComponent2D::setFontProperties( const CFontProperties & fontProp )
{
    if(m_pFontData)
        m_pFontData->m_fontProp.copy( fontProp );
}


/************************************************************************
*    DESC:  Create the font string
************************************************************************/
void CVisualComponent2D::createFontString()
{
    if( m_pFontData )
        createFontString( m_pFontData->m_fontString );
}


/************************************************************************
*    DESC:  Create the font string
*
*    NOTE: Line wrap feature only supported for horizontal left
************************************************************************/
void CVisualComponent2D::createFontString( const std::string & fontString )
{
    // Qualify if we want to build the font string
    if( m_pFontData &&
        !fontString.empty() &&
        !m_pFontData->m_fontProp.m_fontName.empty() &&
        ((fontString != m_pFontData->m_fontString) || (m_vbo == 0)) )
    {
        m_pFontData->m_fontStrSize.clear();
        float lastCharDif(0.f);

        const CFont & font = CFontMgr::Instance().getFont( m_pFontData->m_fontProp.m_fontName );

        m_textureID = font.getTextureID();

        m_pFontData->m_fontString = fontString;

        // count up the number of space characters
        const int spaceCharCount = NGenFunc::CountStrOccurrence( m_pFontData->m_fontString, " " );

        // count up the number of bar | characters
        const int barCharCount = NGenFunc::CountStrOccurrence( m_pFontData->m_fontString, "|" );

        // Size of the allocation
        int charCount = m_pFontData->m_fontString.size() - spaceCharCount - barCharCount;
        m_iboCount = charCount * 6;

        // Set a flag to indicate if the IBO should be built
        const bool BUILD_FONT_IBO = (m_iboCount > CVertBufMgr::Instance().getCurrentMaxFontIndices());

        // Allocate the quad array
        std::unique_ptr<CQuad2D[]> upQuadBuf( new CQuad2D[charCount] );

        // Create a buffer to hold the indices
        std::unique_ptr<uint16_t[]> upIndxBuf;

        // Should we build or rebuild the font IBO
        if( BUILD_FONT_IBO )
            upIndxBuf.reset( new uint16_t[m_iboCount] );

        float xOffset = 0.f;
        float width = 0.f;
        float lineHeightOffset = 0.f;
        float lineHeightWrap = font.getLineHeight() + font.getVertPadding() + m_pFontData->m_fontProp.m_lineWrapHeight;
        float initialHeightOffset = font.getBaselineOffset() + font.getVertPadding();
        float lineSpace = font.getLineHeight() - font.getBaselineOffset();

        uint counter = 0;
        int lineCount = 0;

        // Get the size of the texture
        CSize<float> textureSize = font.getTextureSize();

        // Handle the horizontal alignment
        std::vector<float> lineWidthOffsetVec = calcLineWidthOffset( font, m_pFontData->m_fontString );

        // Set the initial line offset
        xOffset = lineWidthOffsetVec[lineCount++];

        // Handle the vertical alignment
        if( m_pFontData->m_fontProp.m_vAlign == NDefs::EVA_VERT_TOP )
            lineHeightOffset = -initialHeightOffset;

        if( m_pFontData->m_fontProp.m_vAlign == NDefs::EVA_VERT_CENTER )
        {
            lineHeightOffset = -(initialHeightOffset - ((font.getBaselineOffset()-lineSpace) / 2.f) - font.getVertPadding());

            if( lineWidthOffsetVec.size() > 1 )
                lineHeightOffset = ((lineHeightWrap * lineWidthOffsetVec.size()) / 2.f) - font.getBaselineOffset();
        }

        else if( m_pFontData->m_fontProp.m_vAlign == NDefs::EVA_VERT_BOTTOM )
        {
            lineHeightOffset = -(initialHeightOffset - font.getBaselineOffset() - font.getVertPadding());

            if( lineWidthOffsetVec.size() > 1 )
                lineHeightOffset += (lineHeightWrap * (lineWidthOffsetVec.size()-1));
        }

        // Remove any fractional component of the line height offset
        lineHeightOffset = (int)lineHeightOffset;

        // Setup each character in the vertex buffer
        for( size_t i = 0; i < m_pFontData->m_fontString.size(); ++i )
        {
            char id = m_pFontData->m_fontString[i];

            // Line wrap if '|' character was used
            if( id == '|' )
            {
                xOffset = lineWidthOffsetVec[lineCount];
                width = 0.f;

                lineHeightOffset += -lineHeightWrap;
                ++lineCount;
            }
            else
            {
                // See if we can find the character
                const CCharData & charData = font.getCharData(id);

                // Ignore space characters
                if( id != ' ' )
                {
                    CRect<float> rect = charData.rect;

                    float yOffset = (font.getLineHeight() - rect.y2 - charData.offset.h) + lineHeightOffset;

                    // Check if the width or height is odd. If so, we offset
                    // by 0.5 for proper orthographic rendering
                    float additionalOffsetX = 0;
                    if( (int)rect.x2 % 2 != 0 )
                        additionalOffsetX = 0.5f;

                    float additionalOffsetY = 0;
                    if( (int)rect.y2 % 2 != 0 )
                        additionalOffsetY = 0.5f;

                    auto & quadBuf = upQuadBuf[counter];

                    // Calculate the first vertex of the first face
                    quadBuf.vert[0].vert.x = xOffset + charData.offset.w + additionalOffsetX;
                    quadBuf.vert[0].vert.y = yOffset + additionalOffsetY;
                    quadBuf.vert[0].uv.u = rect.x1 / textureSize.w;
                    quadBuf.vert[0].uv.v = (rect.y1 + rect.y2) / textureSize.h;

                    // Calculate the second vertex of the first face
                    quadBuf.vert[1].vert.x = xOffset + rect.x2 + charData.offset.w + additionalOffsetX;
                    quadBuf.vert[1].vert.y = yOffset + rect.y2 + additionalOffsetY;
                    quadBuf.vert[1].uv.u = (rect.x1 + rect.x2) / textureSize.w;
                    quadBuf.vert[1].uv.v = rect.y1 / textureSize.h;

                    // Calculate the third vertex of the first face
                    quadBuf.vert[2].vert.x = quadBuf.vert[0].vert.x;
                    quadBuf.vert[2].vert.y = quadBuf.vert[1].vert.y;
                    quadBuf.vert[2].uv.u = quadBuf.vert[0].uv.u;
                    quadBuf.vert[2].uv.v = quadBuf.vert[1].uv.v;

                    // Calculate the second vertex of the second face
                    quadBuf.vert[3].vert.x = quadBuf.vert[1].vert.x;
                    quadBuf.vert[3].vert.y = quadBuf.vert[0].vert.y;
                    quadBuf.vert[3].uv.u = quadBuf.vert[1].uv.u;
                    quadBuf.vert[3].uv.v = quadBuf.vert[0].uv.v;

                    // Should we build or rebuild the font IBO
                    if( BUILD_FONT_IBO )
                    {
                        // Create the indices into the VBO
                        int arrayIndex = counter * 6;
                        int vertIndex = counter * 4;

                        upIndxBuf[arrayIndex]   = vertIndex;
                        upIndxBuf[arrayIndex+1] = vertIndex+1;
                        upIndxBuf[arrayIndex+2] = vertIndex+2;

                        upIndxBuf[arrayIndex+3] = vertIndex;
                        upIndxBuf[arrayIndex+4] = vertIndex+3;
                        upIndxBuf[arrayIndex+5] = vertIndex+1;
                    }

                    ++counter;
                }

                // Inc the font position
                float inc = charData.xAdvance + m_pFontData->m_fontProp.m_kerning + font.getHorzPadding();

                // Add in any additional spacing for the space character
                if( id == ' ' )
                    inc += m_pFontData->m_fontProp.m_spaceCharKerning;

                width += inc;
                xOffset += inc;

                // Get the longest width of this font string
                if( m_pFontData->m_fontStrSize.w < width )
                {
                    m_pFontData->m_fontStrSize.w = width;

                    // This is the space between this character and the next.
                    // Save this difference so that it can be subtracted at the end
                    lastCharDif = inc - charData.rect.x2;
                }

                // Wrap to another line
                if( (id == ' ') && (m_pFontData->m_fontProp.m_lineWrapWidth > 0.f) )
                {
                    float nextWord = 0.f;

                    // Get the length of the next word to see if if should wrap
                    for( size_t j = i+1; j < m_pFontData->m_fontString.size(); ++j )
                    {
                        id = m_pFontData->m_fontString[j];

                        if( id != '|' )
                        {
                            // See if we can find the character
                            const CCharData & anotherCharData = font.getCharData(id);

                            // Break here when space is found
                            // Don't add the space to the size of the next word
                            if( id == ' ' )
                                break;

                            // Don't count the
                            nextWord += anotherCharData.xAdvance + m_pFontData->m_fontProp.m_kerning + font.getHorzPadding();
                        }
                    }

                    if( width + nextWord >= m_pFontData->m_fontProp.m_lineWrapWidth )
                    {
                        xOffset = lineWidthOffsetVec[lineCount++];
                        width = 0.f;

                        lineHeightOffset += -lineHeightWrap;
                    }
                }
            }
        }

        // Subtract the extra space after the last character
        m_pFontData->m_fontStrSize.w -= lastCharDif;
        m_pFontData->m_fontStrSize.h = font.getLineHeight();

        // Save the data
        // If one doesn't exist, create the VBO and IBO for this font
        /*if( m_vbo == 0 )
            glGenBuffers( 1, &m_vbo );

        glBindBuffer( GL_ARRAY_BUFFER, m_vbo );
        glBufferData( GL_ARRAY_BUFFER, sizeof(CQuad2D) * charCount, upQuadBuf.get(), GL_STATIC_DRAW );
        glBindBuffer( GL_ARRAY_BUFFER, 0 );*/

        // All fonts share the same IBO because it's always the same and the only difference is it's length
        // This updates the current IBO if it exceeds the current max
        m_ibo = CVertBufMgr::Instance().createDynamicFontIBO( CFontMgr::Instance().getGroup(), "dynamic_font_ibo", upIndxBuf.get(), m_iboCount );
    }
    else if( m_pFontData &&
             fontString.empty() &&
             (fontString != m_pFontData->m_fontString) &&
             (m_vbo > 0) )
    {
        m_pFontData->m_fontString.clear();
    }
}


/************************************************************************
*    DESC:  Add up all the character widths
************************************************************************/
std::vector<float> CVisualComponent2D::calcLineWidthOffset(
    const CFont & font,
    const std::string & str )
{
    float firstCharOffset = 0;
    float lastCharOffset = 0;
    float spaceWidth = 0;
    float width = 0;
    int counter = 0;
    std::vector<float> lineWidthOffsetVec;

    for( size_t i = 0; i < str.size(); ++i )
    {
        char id = str[i];

        // Line wrap if '|' character was used
        if( id == '|' )
        {
            // Add the line width to the vector based on horz alignment
            addLineWithToVec( font, lineWidthOffsetVec, m_pFontData->m_fontProp.m_hAlign, width, firstCharOffset, lastCharOffset );

            counter = 0;
            width = 0;
        }
        else
        {
            // Get the next character
            const CCharData & charData = font.getCharData( id );

            if(counter == 0)
                firstCharOffset = charData.offset.w;

            spaceWidth = charData.xAdvance + m_pFontData->m_fontProp.m_kerning + font.getHorzPadding();

            // Add in any additional spacing for the space character
            if( id == ' ' )
                spaceWidth += m_pFontData->m_fontProp.m_spaceCharKerning;

            width += spaceWidth;

            if( id != ' ')
                lastCharOffset = charData.offset.w;

            ++counter;
        }

        // Wrap to another line
        if( (id == ' ') && (m_pFontData->m_fontProp.m_lineWrapWidth > 0.f) )
        {
            float nextWord = 0.f;

            // Get the length of the next word to see if if should wrap
            for( size_t j = i+1; j < str.size(); ++j )
            {
                id = str[j];

                if( id != '|' )
                {
                    // See if we can find the character
                    const CCharData & charData = font.getCharData(id);

                    // Break here when space is found
                    // Don't add the space to the size of the next word
                    if( id == ' ' )
                        break;

                    // Don't count the
                    nextWord += charData.xAdvance + m_pFontData->m_fontProp.m_kerning + font.getHorzPadding();
                }
            }

            if( width + nextWord >= m_pFontData->m_fontProp.m_lineWrapWidth )
            {
                // Add the line width to the vector based on horz alignment
                addLineWithToVec( font, lineWidthOffsetVec, m_pFontData->m_fontProp.m_hAlign, width-spaceWidth, firstCharOffset, lastCharOffset );

                counter = 0;
                width = 0;
            }
        }
    }

    // Add the line width to the vector based on horz alignment
    addLineWithToVec( font, lineWidthOffsetVec, m_pFontData->m_fontProp.m_hAlign, width, firstCharOffset, lastCharOffset );

    return lineWidthOffsetVec;
}


/************************************************************************
*    DESC:  Add the line width to the vector based on horz alignment
************************************************************************/
void CVisualComponent2D::addLineWithToVec(
    const CFont & font,
    std::vector<float> & lineWidthOffsetVec,
    const NDefs::EHorzAlignment hAlign,
    float width,
    float firstCharOffset,
    float lastCharOffset )
{
    if( hAlign == NDefs::EHA_HORZ_LEFT )
        lineWidthOffsetVec.push_back(-(firstCharOffset + font.getHorzPadding()));

    else if( hAlign == NDefs::EHA_HORZ_CENTER )
        lineWidthOffsetVec.push_back(-((width - font.getHorzPadding()) / 2.f));

    else if( hAlign == NDefs::EHA_HORZ_RIGHT )
        lineWidthOffsetVec.push_back(-(width - lastCharOffset - font.getHorzPadding()));

    // Remove any fractional component of the last index
    lineWidthOffsetVec.back() = (int)lineWidthOffsetVec.back();
}


/************************************************************************
*    DESC:  Get/Set the displayed font string
************************************************************************/
const std::string & CVisualComponent2D::getFontString()
{
    if( !m_pFontData )
    {
        throw NExcept::CCriticalException("Not a sprite font!",
            boost::str( boost::format("Can't ask for the font string from a sprite that is not a sprite font.\n\n%s\nLine: %s")
                % __FUNCTION__ % __LINE__ ));
    }

    return m_pFontData->m_fontString;
}

void CVisualComponent2D::setFontString( const std::string & fontString )
{
    if(m_pFontData)
        m_pFontData->m_fontString = fontString;
}


/************************************************************************
*    DESC:  Set/Get the color
************************************************************************/
void CVisualComponent2D::setColor( const CColor & color )
{
    m_color = color;
}

void CVisualComponent2D::setColor( float r, float g, float b, float a )
{
    // This function assumes values between 0.0 to 1.0.
    m_color.set( r, g, b, a );
}

const CColor & CVisualComponent2D::getColor() const
{
    return m_color;
}


/************************************************************************
*    DESC:  Set/Get the default color
************************************************************************/
void CVisualComponent2D::setDefaultColor()
{
    m_color = m_rVisualData.getColor();
}

const CColor & CVisualComponent2D::getDefaultColor() const
{
    return m_rVisualData.getColor();
}


/************************************************************************
*    DESC:  Set/Get the alpha
************************************************************************/
void CVisualComponent2D::setAlpha( float alpha, bool allowToExceed )
{
    if( alpha > 1.5 )
        alpha *= defs_RGB_TO_DEC;
    
    if( allowToExceed || (alpha < m_rVisualData.getColor().a) )
        m_color.a = alpha;
    else
        m_color.a = m_rVisualData.getColor().a;
}

float CVisualComponent2D::getAlpha() const
{
    return m_color.a;
}


/************************************************************************
*    DESC:  Set/Get the default alpha
************************************************************************/
void CVisualComponent2D::setDefaultAlpha()
{
    m_color.a = m_rVisualData.getColor().a;
}

float CVisualComponent2D::getDefaultAlpha() const
{
    return m_rVisualData.getColor().a;
}


/************************************************************************
*    DESC:  Set the frame ID from index
************************************************************************/
void CVisualComponent2D::setFrame( uint index )
{
    if( GENERATION_TYPE == NDefs::EGT_SPRITE_SHEET )
    {
        auto rGlyph = m_rVisualData.getSpriteSheet().getGlyph( index );
        m_glyphUV = rGlyph.getUV();
        m_quadVertScale = rGlyph.getSize() * m_rVisualData.getDefaultUniformScale();
    }
    else
        m_textureID = m_rVisualData.getTextureID( index );

    m_frameIndex = index;
}


/************************************************************************
*    DESC:  Get the current frame index
************************************************************************/
uint CVisualComponent2D::getCurrentFrame() const
{
    return m_frameIndex;
}


/************************************************************************
*    DESC:  Is rendering allowed?
************************************************************************/
bool CVisualComponent2D::allowRender()
{
    return ((GENERATION_TYPE > NDefs::EGT_NULL) && (GENERATION_TYPE < NDefs::EGT_FONT)) ||
           ((GENERATION_TYPE == NDefs::EGT_FONT) && !m_pFontData->m_fontString.empty() && (m_vbo > 0));
}


/************************************************************************
*    DESC:  Is this a font sprite
************************************************************************/
bool CVisualComponent2D::isFontSprite()
{
    return (GENERATION_TYPE == NDefs::EGT_FONT);
}


/************************************************************************
*    DESC:  Get the font size
************************************************************************/
const CSize<float> & CVisualComponent2D::getFontSize() const
{
    if( !m_pFontData )
    {
        throw NExcept::CCriticalException("Not a sprite font!",
            boost::str( boost::format("Can't ask for the font size from a sprite that is not a sprite font.\n\n%s\nLine: %s")
                % __FUNCTION__ % __LINE__ ));
    }

    return m_pFontData->m_fontStrSize;
}
