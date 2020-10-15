
/************************************************************************
*    FILE NAME:       ivisualcomponent.h
*
*    DESCRIPTION:     iVisualComponent Class
************************************************************************/

#pragma once

// Physical component dependency
#include <common/visual.h>

// Game lib dependencies
#include <common/color.h>
#include <common/size.h>
#include <common/defs.h>
#include <utilities/matrix.h>

// Standard lib dependencies
#include <string>

// Vulkan lib dependencies
#include <system/vulkan.h>

// Forward declaration(s)
class CMatrix;
class iObjectData;
class CFontProperties;
class CFontData;
class CCamera;
class CObject;
struct XMLNode;

class iVisualComponent : public CVisual
{
public:

    // Constructor
    iVisualComponent( const iObjectData & objectData );
    
    // Destructor
    virtual ~iVisualComponent() {}
    
    // Record the command buffers
    virtual void recordCommandBuffer( 
        uint32_t index,
        VkCommandBuffer cmdBuffer,
        const CObject * const pObject,
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
    virtual const CSize<float> & getSize() const { return m_null_float_size; };
    
    // Is this a font sprite
    bool isFontSprite() const;
    
    // Get the generation type
    EGenType getGenerationType() const;
    
    // Get the crop offset
    virtual const CSize<int> & getCropOffset( uint index = 0 ) const;

    // Get the rotation matrix
    virtual const CMatrix & getRotMatrix() const { return m_null_matrix; };
    virtual CMatrix & getRotMatrix() { return m_null_matrix; };

    // Use a point to set a column - used for 3d physics
    virtual void setRotMatrixColumn( const int col, const float x, const float y, const float z ){};
    
protected:

    // Generation type
    const EGenType GENERATION_TYPE;
    
    // Color
    CColor m_color;
    
    // The default color
    const CColor m_rDefaultColor;
    
    // Frame index
    uint32_t m_frameIndex;
    
    // Static null data members
    static std::string m_null_string;
    static CSize<int> m_null_int_size;
    static CSize<float> m_null_float_size;
    static CMatrix m_null_matrix;
};
