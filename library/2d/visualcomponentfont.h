
/************************************************************************
*    FILE NAME:       visualcomponentfont.h
*
*    DESCRIPTION:     Class for handling the visual part of a sprite
************************************************************************/

#ifndef __visual_component_font_h__
#define __visual_component_font_h__

// Physical component dependency
#include <2d/visualcomponentquad.h>

// Game lib dependencies
#include <common/fontdata.h>
#include <common/memorybuffer.h>

// Forward declaration(s)
class CFont;

class CVisualComponentFont : public CVisualComponentQuad
{
public:

    // Constructor
    CVisualComponentFont( const CObjectData2D & objectData );

    // Destructor
    virtual ~CVisualComponentFont();
    
    // Set the font data
    void setFontData( const CFontData & fontData );
    
    // Load the font properties from XML node
    void loadFontPropFromNode( const XMLNode & node );

    // Set the font properties
    void setFontProperties( const CFontProperties & fontProp ) override;
    
    // Set the string to display
    void createFontString() override;
    void createFontString( const std::string & fontString ) override;

    // Get the displayed font string
    const std::string & getFontString() override;
    void setFontString( const std::string & fontString ) override;
    
    // Get the font size
    const CSize<float> & getFontSize() const override;
    
    // Record the command buffers
    void recordCommandBuffer( 
        uint32_t index,
        VkCommandBuffer cmdBuffer,
        const CMatrix & model,
        const CMatrix & viewProj ) override;
    
private:
    
    // Update the UBO buffer
    void updateUBO(
        uint32_t index,
        CDevice & device,
        const iObjectVisualData & rVisualData,
        const CMatrix & model,
        const CMatrix & viewProj ) override;
    
    // Add the line width to the vector based on horz alignment
    void addLineWithToVec(
        const CFont & font,
        std::vector<float> & lineWidthOffsetVec,
        const NDefs::EHorzAlignment hAlign,
        float width,
        float firstCharOffset,
        float lastCharOffset );

    // Add up all the character widths
    std::vector<float> calcLineWidthOffset(
        const CFont & font,
        const std::string & str);
    
    // Is recording the command buffer allowed?
    bool allowCommandRecording() override;
    
private:
        
    // Unique pointer for font data
    CFontData m_fontData;
    
    // VBO buffer
    CMemoryBuffer m_vboBuffer;
    
    // ibo count
    size_t m_iboCount;
};

#endif  // __visual_component_font_h__

