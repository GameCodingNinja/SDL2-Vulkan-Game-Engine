
/************************************************************************
*    FILE NAME:       visualcomponentspritesheet.h
*
*    DESCRIPTION:     Class for handling the visual part of a sprite
************************************************************************/

#ifndef __visual_component_spritesheet_h__
#define __visual_component_spritesheet_h__

// Physical component dependency
#include <2d/visualcomponentquad.h>

class CVisualComponentSpriteSheet : public CVisualComponentQuad
{
public:

    // Constructor
    CVisualComponentSpriteSheet( const iObjectData & objectData );

    // Destructor
    virtual ~CVisualComponentSpriteSheet();
    
    // Set the frame index
    void setFrame( uint index ) override;
    
private:
    
    // Update the UBO buffer
    void updateUBO(
        uint32_t index,
        CDevice & device,
        const iObjectVisualData & rVisualData,
        const CObjectTransform * const pObject,
        const CCamera & camera ) override;
    
    // Get the crop offset
    const CSize<int> & getCropOffset( uint index = 0 ) const override;
    
private:
    
    // Sprite sheet Glyph UV
    CRect<float> m_glyphUV;
};

#endif
