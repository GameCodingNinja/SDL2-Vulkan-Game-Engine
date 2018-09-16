
/************************************************************************
*    FILE NAME:       ivisualcomponent.h
*
*    DESCRIPTION:     iVisualComponent Class
************************************************************************/

#ifndef __i_visual_component_h__
#define __i_visual_component_h__

// Game lib dependencies
#include <common/color.h>
#include <common/size.h>
#include <common/defs.h>

// Standard lib dependencies
#include <string>

// Vulkan lib dependencies
#include <system/vulkan.h>

// Forward declaration(s)
class CColor;
class CMatrix;
class CObjectVisualData2D;
class CObjectVisualData3D;

class iVisualComponent
{
public:
    
    // Constructor
    iVisualComponent( const CObjectVisualData2D & visualData );
    iVisualComponent( const CObjectVisualData3D & visualData );
    
    // Destructor
    virtual ~iVisualComponent() {}
    
    // Record the command buffers
    virtual void recordCommandBuffers(
        uint32_t cmdBufIndex,
        const CMatrix & model,
        const CMatrix & viewProj ) {}
    
    virtual void recordCommandBuffers( 
        uint32_t index,
        VkCommandBuffer cmdBuffer,
        const CMatrix & model,
        const CMatrix & viewProj ) {}

    // do the render
    virtual void render( const CMatrix & objMatrix, const CMatrix & matrix ) {}
    
    // Set/Get the color
    void setColor( const CColor & color );
    void setColor( float r, float g, float b, float a );
    const CColor & getColor() const;
    void setDefaultColor();
    const CColor & getDefaultColor() const;
    
    // Set/Get additive color
    static void setAdditiveColor( const CColor & color );
    static void setAdditiveColor( float r, float g, float b, float a );
    static const CColor & getAdditiveColor();
    
    // Set/Get the alpha
    void setAlpha( float alpha, bool allowToExceed = false );
    float getAlpha() const;
    void setDefaultAlpha();
    float getDefaultAlpha() const;
    
    // Set the frame index
    virtual void setFrame( uint index );
    
    // Get the current frame index
    virtual uint getCurrentFrame() const;

    // Set the string to display
    virtual void createFontString(){}
    virtual void createFontString( const std::string & fontString ){}

    // Get the displayed font string
    virtual const std::string & getFontString() { return m_stringDummy; };
    virtual void setFontString( const std::string & fontString ){}
    
    // Get the font size
    virtual const CSize<float> & getFontSize() const { return m_sizeDummy; };
    
    // Is this a font sprite
    bool isFontSprite() const;
    
protected:

    // Generation type
    const NDefs::EGenerationType GENERATION_TYPE;

    // The scale of the quad
    CSize<float> m_quadVertScale;
    
    // Color
    CColor m_color;
    
    // Additive Color
    static CColor m_additive;
    
    // The default color
    const CColor & m_rDefaultColor;
    
    // Frame index
    uint32_t m_frameIndex;
    
    // String dummy
    static std::string m_stringDummy;
    static CSize<float> m_sizeDummy;
};

#endif
