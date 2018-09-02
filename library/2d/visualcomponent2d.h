
/************************************************************************
*    FILE NAME:       visualcomponent2d.h
*
*    DESCRIPTION:     Class for handling the visual part of the sprite
************************************************************************/

#ifndef __visual_component_2d_h__
#define __visual_component_2d_h__

// Physical component dependency
#include <common/ivisualcomponent.h>

// Game lib dependencies
#include <utilities/matrix.h>
#include <common/point.h>
#include <common/defs.h>
#include <common/rect.h>
#include <common/size.h>
#include <utilities/xmlParser.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <string>
#include <memory>

// Forward declaration(s)
class CObjectVisualData2D;
class CFont;
class CShaderData;
class CFontData;
class CFontProperties;

class CVisualComponent2D : public iVisualComponent, boost::noncopyable
{
public:

    // Constructor
    CVisualComponent2D( const CObjectVisualData2D & visualData );

    // Destructor
    virtual ~CVisualComponent2D();
    
    // Delete the custom VBO for this font
    void deleteFontVBO();

    // do the render
    void render( const CMatrix & objMatrix, const CMatrix & matrix ) override;
    
    // Set the font data
    void setFontData( const CFontData & fontData );
    
    // Load the font properties from XML node
    void loadFontPropFromNode( const XMLNode & node );

    // Set the font properties
    void setFontProperties( const CFontProperties & fontProp );
    
    // NOTE: Interface overridden member functions
    
    // Set/Get the color
    /*void setColor( const CColor & color ) override;
    void setColor( float r, float g, float b, float a ) override;
    const CColor & getColor() const override;
    void setDefaultColor() override;
    const CColor & getDefaultColor() const override;

    // Set/Get the alpha
    void setAlpha( float alpha, bool allowToExceed = false ) override;
    float getAlpha() const override;
    void setDefaultAlpha() override;
    float getDefaultAlpha() const override;*/
    
    // Set the string to display
    void createFontString() override;
    void createFontString( const std::string & fontString ) override;

    // Get the displayed font string
    const std::string & getFontString() override;
    void setFontString( const std::string & fontString ) override;
    
    // Get the font size
    const CSize<float> & getFontSize() const override;

private:

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
    
    // Is rendering allowed?
    bool allowRender();

private:
    
    // Shader data pointer - We DON'T own this pointer, don't free
    CShaderData * m_pShaderData;

    // VBO
    uint32_t m_vbo;

    // IBO
    uint32_t m_ibo;

    // Loaded texture data
    uint32_t m_textureID;

    // shader location data
    int32_t m_vertexLocation;
    int32_t m_uvLocation;
    int32_t m_text0Location;
    int32_t m_colorLocation;
    int32_t m_matrixLocation;
    int32_t m_glyphLocation;

    // The scale of the quad
    CSize<float> m_quadVertScale;

    // Reference to object visual data
    const CObjectVisualData2D & m_rVisualData;

    // IBO count
    uint16_t m_iboCount;

    // OpenGL draw mode
    uint32_t m_drawMode;

    // IBO indice type
    uint32_t m_indiceType;
    
    // Sprite sheet Glyph UV
    CRect<float> m_glyphUV;

    ///////////////////////
    //  Font data types
    ///////////////////////

    // Unique pointer for font data
    CFontData * m_pFontData;
    
};

#endif  // __visual_component_2d_h__

