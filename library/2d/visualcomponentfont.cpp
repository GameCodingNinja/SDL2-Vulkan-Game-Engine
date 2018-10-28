
/************************************************************************
*    FILE NAME:       visualcomponentfont.cpp
*
*    DESCRIPTION:     Class for handling the visual part of a sprite
************************************************************************/

// Physical component dependency
#include <2d/visualcomponentfont.h>

// Game lib dependencies
#include <objectdata/objectdata2d.h>
#include <objectdata/iobjectvisualdata.h>
#include <common/uniformbufferobject.h>
#include <common/quad2d.h>
#include <common/pipeline.h>
#include <system/device.h>
#include <managers/fontmanager.h>
#include <utilities/genfunc.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CVisualComponentFont::CVisualComponentFont( const CObjectData2D & objectData ) :
    CVisualComponentQuad( objectData )
{
}


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CVisualComponentFont::~CVisualComponentFont()
{
    CDevice::Instance().freeMemoryBuffer( m_vboBuffer );
}

/************************************************************************
*    DESC:  Update the UBO buffer
************************************************************************/
void CVisualComponentFont::updateUBO(
    uint32_t index,
    CDevice & device,
    const iObjectVisualData & rVisualData,
    const CMatrix & model,
    const CMatrix & viewProj )
{
    // Setup the uniform buffer object
    NUBO::model_viewProj_color_additive ubo;
    ubo.model = model;
    ubo.viewProj = viewProj;
    ubo.color = m_color;
    ubo.additive = m_additive;

    // Update the uniform buffer
    device.updateUniformBuffer( ubo, m_uniformBufVec[index].m_deviceMemory );
}


/***************************************************************************
*   DESC:  Record the command buffers
****************************************************************************/
void CVisualComponentFont::recordCommandBuffer(
    uint32_t index,
    VkCommandBuffer cmdBuffer,
    const CMatrix & model,
    const CMatrix & viewProj )
{
    if( allowCommandRecording() )
    {
        const auto & rVisualData( m_rObjectData.getVisualData() );
        auto & device( CDevice::Instance() );

        // Get the pipeline data
        const CPipelineData & rPipelineData = device.getPipelineData( rVisualData.getPipelineIndex() );

        // Update the UBO buffer
        updateUBO( index, device, rVisualData, model, viewProj );

        //CDevice::Instance().beginCommandBuffer( index, m_commandBufVec[index] );

        // Bind the pipeline
        if( device.getLastPipeline() != rPipelineData.m_pipeline )
        {
            vkCmdBindPipeline( cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, rPipelineData.m_pipeline );
            device.setLastPipeline( rPipelineData.m_pipeline );
        }


        /*VkViewport viewport = {0, 0, 1280, 720, 0.0f, 1.0f};
        vkCmdSetViewport(m_commandBufVec[index], 0, 1, &viewport );

        VkRect2D scissor = {{50, 50}, {1000, 500}};
        vkCmdSetScissor( m_commandBufVec[index], 0, 1, &scissor );*/

        // Bind vertex buffer
        VkBuffer vertexBuffers[] = {m_vboBuffer.m_buffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers( cmdBuffer, 0, 1, vertexBuffers, offsets );

        // Bind the index buffer
        vkCmdBindIndexBuffer( cmdBuffer, device.getSharedFontIBO().m_buffer, 0, VK_INDEX_TYPE_UINT16 );

        // Use the push descriptors
        m_pushDescSet.cmdPushDescriptorSet( index, cmdBuffer, rPipelineData.m_pipelineLayout );

        // Do the draw
        vkCmdDrawIndexed( cmdBuffer, m_iboCount, 1, 0, 0, 0 );
    }
    
    //CDevice::Instance().endCommandBuffer( m_commandBufVec[index] );
}


/************************************************************************
*    DESC:  Load the font properties from XML node
************************************************************************/
void CVisualComponentFont::loadFontPropFromNode( const XMLNode & node )
{
    m_fontData.loadFromNode( node );
}


/************************************************************************
*    DESC:  Set the font data
************************************************************************/
void CVisualComponentFont::setFontData( const CFontData & fontData )
{
    m_fontData.copy( fontData );
}


/************************************************************************
*    DESC:  Set the font properties
************************************************************************/
void CVisualComponentFont::setFontProperties( const CFontProperties & fontProp )
{
    m_fontData.m_fontProp.copy( fontProp );
}


/************************************************************************
*    DESC:  Create the font string
************************************************************************/
void CVisualComponentFont::createFontString()
{
    createFontString( m_fontData.m_fontString );
}


/************************************************************************
*    DESC:  Create the font string
*
*    NOTE: Line wrap feature only supported for horizontal left
************************************************************************/
void CVisualComponentFont::createFontString( const std::string & fontString )
{
    // Qualify if we want to build the font string
    if( !fontString.empty() &&
        !m_fontData.m_fontProp.m_fontName.empty() &&
        ((fontString != m_fontData.m_fontString)) )
    {
        m_fontData.m_fontStrSize.clear();
        float lastCharDif(0.f);
        auto & device( CDevice::Instance() );

        const CFont & font = CFontMgr::Instance().getFont( m_fontData.m_fontProp.m_fontName );

        m_fontData.m_fontString = fontString;

        // count up the number of space characters
        const int spaceCharCount = NGenFunc::CountStrOccurrence( m_fontData.m_fontString, " " );

        // count up the number of bar | characters
        const int barCharCount = NGenFunc::CountStrOccurrence( m_fontData.m_fontString, "|" );

        // Size of the allocation
        size_t charCount = m_fontData.m_fontString.size() - spaceCharCount - barCharCount;
        m_iboCount = charCount * 6;

        // Set a flag to indicate if the IBO should be built
        const bool BUILD_FONT_IBO = (m_iboCount > device.getSharedFontIBOMaxIndiceCount());

        // Allocate the quad array
        std::vector<CQuad2D> quadVec( charCount );

        // Create a buffer to hold the indices
        std::vector<uint16_t> iboVec( m_iboCount );

        float xOffset = 0.f;
        float width = 0.f;
        float lineHeightOffset = 0.f;
        float lineHeightWrap = font.getLineHeight() + font.getVertPadding() + m_fontData.m_fontProp.m_lineWrapHeight;
        float initialHeightOffset = font.getBaselineOffset() + font.getVertPadding();
        float lineSpace = font.getLineHeight() - font.getBaselineOffset();

        uint counter = 0;
        int lineCount = 0;

        // Get the size of the texture
        CSize<float> textureSize = font.getTextureSize();

        // Handle the horizontal alignment
        std::vector<float> lineWidthOffsetVec = calcLineWidthOffset( font, m_fontData.m_fontString );

        // Set the initial line offset
        xOffset = lineWidthOffsetVec[lineCount++];

        // Handle the vertical alignment
        if( m_fontData.m_fontProp.m_vAlign == NDefs::EVA_VERT_TOP )
            lineHeightOffset = initialHeightOffset - font.getBaselineOffset();

        if( m_fontData.m_fontProp.m_vAlign == NDefs::EVA_VERT_CENTER )
        {
            lineHeightOffset = -(initialHeightOffset - ((font.getBaselineOffset()-lineSpace) / 2.f) - font.getVertPadding());

            if( lineWidthOffsetVec.size() > 1 )
                lineHeightOffset = -((lineHeightWrap * lineWidthOffsetVec.size()) / 2.f);
        }

        else if( m_fontData.m_fontProp.m_vAlign == NDefs::EVA_VERT_BOTTOM )
        {
            lineHeightOffset = -(initialHeightOffset - font.getBaselineOffset() - font.getVertPadding());

            if( lineWidthOffsetVec.size() > 1 )
                lineHeightOffset += -((lineHeightWrap * (lineWidthOffsetVec.size()-1)) + font.getBaselineOffset());
        }

        // Remove any fractional component of the line height offset
        lineHeightOffset = (int)lineHeightOffset;

        // Setup each character in the vertex buffer
        for( size_t i = 0; i < m_fontData.m_fontString.size(); ++i )
        {
            char id = m_fontData.m_fontString[i];

            // Line wrap if '|' character was used
            if( id == '|' )
            {
                xOffset = lineWidthOffsetVec[lineCount];
                width = 0.f;

                lineHeightOffset += lineHeightWrap;
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

                    float yOffset = lineHeightOffset + charData.offset.h;

                    // Check if the width or height is odd. If so, we offset
                    // by 0.5 for proper orthographic rendering
                    float additionalOffsetX = 0;
                    if( (int)rect.x2 % 2 != 0 )
                        additionalOffsetX = 0.5f;

                    float additionalOffsetY = 0;
                    if( (int)rect.y2 % 2 != 0 )
                        additionalOffsetY = 0.5f;

                    auto & quadBuf = quadVec[counter];

                    // Calculate the second vertex of the first face
                    quadBuf.vert[1].vert.x = xOffset + charData.offset.w + additionalOffsetX;
                    quadBuf.vert[1].vert.y = yOffset + additionalOffsetY;
                    quadBuf.vert[1].uv.u = rect.x1 / textureSize.w;
                    quadBuf.vert[1].uv.v = rect.y1 / textureSize.h;
                    
                    // Calculate the forth vertex of the first face
                    quadBuf.vert[3].vert.x = xOffset + rect.x2 + charData.offset.w + additionalOffsetX;
                    quadBuf.vert[3].vert.y = yOffset + rect.y2 + additionalOffsetY;
                    quadBuf.vert[3].uv.u = (rect.x1 + rect.x2) / textureSize.w;
                    quadBuf.vert[3].uv.v = (rect.y1 + rect.y2) / textureSize.h;
                    
                    // Calculate the first vertex of the first face
                    quadBuf.vert[0].vert.x = quadBuf.vert[3].vert.x;
                    quadBuf.vert[0].vert.y = quadBuf.vert[1].vert.y;
                    quadBuf.vert[0].uv.u = quadBuf.vert[3].uv.u;
                    quadBuf.vert[0].uv.v = quadBuf.vert[1].uv.v;

                    // Calculate the third vertex of the second face
                    quadBuf.vert[2].vert.x = quadBuf.vert[1].vert.x;
                    quadBuf.vert[2].vert.y = quadBuf.vert[3].vert.y;
                    quadBuf.vert[2].uv.u = quadBuf.vert[1].uv.u;
                    quadBuf.vert[2].uv.v = quadBuf.vert[3].uv.v;

                    // Should we build or rebuild the font IBO
                    if( BUILD_FONT_IBO )
                    {
                        // Create the indices into the VBO
                        int arrayIndex = counter * 6;
                        int vertIndex = counter * 4;

                        iboVec[arrayIndex]   = vertIndex;
                        iboVec[arrayIndex+1] = vertIndex+1;
                        iboVec[arrayIndex+2] = vertIndex+2;

                        iboVec[arrayIndex+3] = vertIndex+2;
                        iboVec[arrayIndex+4] = vertIndex+3;
                        iboVec[arrayIndex+5] = vertIndex;
                    }

                    ++counter;
                }

                // Inc the font position
                float inc = charData.xAdvance + m_fontData.m_fontProp.m_kerning + font.getHorzPadding();

                // Add in any additional spacing for the space character
                if( id == ' ' )
                    inc += m_fontData.m_fontProp.m_spaceCharKerning;

                width += inc;
                xOffset += inc;

                // Get the longest width of this font string
                if( m_fontData.m_fontStrSize.w < width )
                {
                    m_fontData.m_fontStrSize.w = width;

                    // This is the space between this character and the next.
                    // Save this difference so that it can be subtracted at the end
                    lastCharDif = inc - charData.rect.x2;
                }

                // Wrap to another line
                if( (id == ' ') && (m_fontData.m_fontProp.m_lineWrapWidth > 0.f) )
                {
                    float nextWord = 0.f;

                    // Get the length of the next word to see if if should wrap
                    for( size_t j = i+1; j < m_fontData.m_fontString.size(); ++j )
                    {
                        id = m_fontData.m_fontString[j];

                        if( id != '|' )
                        {
                            // See if we can find the character
                            const CCharData & anotherCharData = font.getCharData(id);

                            // Break here when space is found
                            // Don't add the space to the size of the next word
                            if( id == ' ' )
                                break;

                            // Don't count the
                            nextWord += anotherCharData.xAdvance + m_fontData.m_fontProp.m_kerning + font.getHorzPadding();
                        }
                    }

                    if( width + nextWord >= m_fontData.m_fontProp.m_lineWrapWidth )
                    {
                        xOffset = lineWidthOffsetVec[lineCount++];
                        width = 0.f;

                        lineHeightOffset += -lineHeightWrap;
                    }
                }
            }
        }

        // Subtract the extra space after the last character
        m_fontData.m_fontStrSize.w -= lastCharDif;
        m_fontData.m_fontStrSize.h = font.getLineHeight();
        
        // Free the previous memory buffer
        device.freeMemoryBuffer( m_vboBuffer );
        
        // Create the font vertex buffer
        device.creatMemoryBuffer( quadVec, m_vboBuffer, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT );
        
        // Create the font IBO vector
        // All fonts share the same IBO because it's always the same and the only difference is it's length
        // This updates the current IBO if it exceeds the current max
        if( BUILD_FONT_IBO )
            device.createSharedFontIBO( iboVec );
        
        const uint32_t pipelineIndex( m_rObjectData.getVisualData().getPipelineIndex() );
        
        device.createPushDescriptorSet(
            pipelineIndex,
            font.getTexture(),
            m_uniformBufVec,
            m_pushDescSet );
        
        //m_textureID = font.getTextureID();

        // Save the data
        // If one doesn't exist, create the VBO and IBO for this font
        /*if( m_vbo == 0 )
            glGenBuffers( 1, &m_vbo );

        glBindBuffer( GL_ARRAY_BUFFER, m_vbo );
        glBufferData( GL_ARRAY_BUFFER, sizeof(CQuad2D) * charCount, upQuadBuf.get(), GL_STATIC_DRAW );
        glBindBuffer( GL_ARRAY_BUFFER, 0 );*/

        // All fonts share the same IBO because it's always the same and the only difference is it's length
        // This updates the current IBO if it exceeds the current max
        //m_ibo = CVertBufMgr::Instance().createDynamicFontIBO( CFontMgr::Instance().getGroup(), "dynamic_font_ibo", upIndxBuf.get(), m_iboCount );
    }
    else if( fontString.empty() &&
             (fontString != m_fontData.m_fontString) &&
             (!m_vboBuffer.isEmpty()) )
    {
        m_fontData.m_fontString.clear();
    }
}


/************************************************************************
*    DESC:  Add up all the character widths
************************************************************************/
std::vector<float> CVisualComponentFont::calcLineWidthOffset(
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
            addLineWithToVec( font, lineWidthOffsetVec, m_fontData.m_fontProp.m_hAlign, width, firstCharOffset, lastCharOffset );

            counter = 0;
            width = 0;
        }
        else
        {
            // Get the next character
            const CCharData & charData = font.getCharData( id );

            if(counter == 0)
                firstCharOffset = charData.offset.w;

            spaceWidth = charData.xAdvance + m_fontData.m_fontProp.m_kerning + font.getHorzPadding();

            // Add in any additional spacing for the space character
            if( id == ' ' )
                spaceWidth += m_fontData.m_fontProp.m_spaceCharKerning;

            width += spaceWidth;

            if( id != ' ')
                lastCharOffset = charData.offset.w;

            ++counter;
        }

        // Wrap to another line
        if( (id == ' ') && (m_fontData.m_fontProp.m_lineWrapWidth > 0.f) )
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
                    nextWord += charData.xAdvance + m_fontData.m_fontProp.m_kerning + font.getHorzPadding();
                }
            }

            if( width + nextWord >= m_fontData.m_fontProp.m_lineWrapWidth )
            {
                // Add the line width to the vector based on horz alignment
                addLineWithToVec( font, lineWidthOffsetVec, m_fontData.m_fontProp.m_hAlign, width-spaceWidth, firstCharOffset, lastCharOffset );

                counter = 0;
                width = 0;
            }
        }
    }

    // Add the line width to the vector based on horz alignment
    addLineWithToVec( font, lineWidthOffsetVec, m_fontData.m_fontProp.m_hAlign, width, firstCharOffset, lastCharOffset );

    return lineWidthOffsetVec;
}


/************************************************************************
*    DESC:  Add the line width to the vector based on horz alignment
************************************************************************/
void CVisualComponentFont::addLineWithToVec(
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
const std::string & CVisualComponentFont::getFontString()
{
    return m_fontData.m_fontString;
}

void CVisualComponentFont::setFontString( const std::string & fontString )
{
    m_fontData.m_fontString = fontString;
}


/************************************************************************
*    DESC:  Get the font size
************************************************************************/
const CSize<float> & CVisualComponentFont::getFontSize() const
{
    return m_fontData.m_fontStrSize;
}


/************************************************************************
*    DESC:  Is recording the command buffer allowed?
************************************************************************/
bool CVisualComponentFont::allowCommandRecording()
{
    return CVisualComponentQuad::allowCommandRecording() ||
        ((GENERATION_TYPE == NDefs::EGT_FONT) && !m_fontData.m_fontString.empty() && !m_vboBuffer.isEmpty());
}