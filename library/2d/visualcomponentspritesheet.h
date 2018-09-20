
/************************************************************************
*    FILE NAME:       visualcomponentspritesheet.h
*
*    DESCRIPTION:     Class for handling the visual part of 2d quad
************************************************************************/

#ifndef __visual_component_spritesheet_h__
#define __visual_component_spritesheet_h__

// Physical component dependency
#include <2d/visualcomponentquad.h>

// Game lib dependencies
#include <common/rect.h>

class CVisualComponentSpriteSheet : public CVisualComponentQuad
{
public:

    // Constructor
    CVisualComponentSpriteSheet( const CObjectData2D & objectData );

    // Destructor
    virtual ~CVisualComponentSpriteSheet();
    
private:
    
    // Update the UBO buffer
    void updateUBO(
        uint32_t index,
        CDevice & device,
        const CObjectVisualData2D & rVisualData,
        const CMatrix & model,
        const CMatrix & viewProj ) override;
    
    // Get the crop offset
    const CSize<int> & getCropOffset( uint index = 0 ) const override;
    
private:
    
    // Sprite sheet Glyph UV
    CRect<float> m_glyphUV;
};

#endif  // __visual_component_spritesheet_h__


