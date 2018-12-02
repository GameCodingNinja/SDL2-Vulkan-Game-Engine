
/************************************************************************
*    FILE NAME:       ivisualcomponent.h
*
*    DESCRIPTION:     iVisualComponent Class
************************************************************************/

#ifndef __i_visual_component_h__
#define __i_visual_component_h__

// Physical component dependency
#include <common/visual.h>

// Game lib dependencies
#include <common/color.h>
#include <common/size.h>
#include <common/defs.h>

// Standard lib dependencies
#include <string>

// Vulkan lib dependencies
#include <system/vulkan.h>

// Forward declaration(s)
class CMatrix;
class CObjectData2D;
class CObjectData3D;
class CFontProperties;
class CFontData;
class CCamera;
class CObject2D;
struct XMLNode;

class iVisualComponent : public CVisual
{
public:
    
    // Constructor
    iVisualComponent( const CObjectData2D & objectData );
    iVisualComponent( const CObjectData3D & objectData );
    
    // Destructor
    virtual ~iVisualComponent() {}
    
    // Record the command buffers
    virtual void recordCommandBuffer( 
        uint32_t index,
        VkCommandBuffer cmdBuffer,
        const CObject2D * const pObject,
        const CCamera & camera ) {}
    
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
    
    // Load the font properties from XML node
    virtual void loadFontPropFromNode( const XMLNode & node ){}
    
    // Set the font data
    virtual void setFontData( const CFontData & fontData ){}
    
    // Set the font properties
    virtual void setFontProperties( const CFontProperties & fontProp ){}

    // Set the string to display
    virtual void createFontString(){}
    virtual void createFontString( const std::string & fontString ){}

    // Get the displayed font string
    virtual const std::string & getFontString() { return m_null_string; };
    virtual void setFontString( const std::string & fontString ){}
    
    // Get the size
    virtual const CSize<float> & getSize() const;
    
    // Is this a font sprite
    bool isFontSprite() const;
    
    // Get the generation type
    NDefs::EGenerationType getGenerationType() const;
    
    // Get the crop offset
    virtual const CSize<int> & getCropOffset( uint index = 0 ) const;
    
protected:

    // Generation type
    const NDefs::EGenerationType GENERATION_TYPE;

    // The scale of the quad
    CSize<float> m_quadVertScale;
    
    // Color
    CColor m_color;
    
    // The default color
    const CColor m_rDefaultColor;
    
    // Frame index
    uint32_t m_frameIndex;
    
    // Static null data members
    static std::string m_null_string;
    static CSize<int> m_null_int_size;
};

#endif
