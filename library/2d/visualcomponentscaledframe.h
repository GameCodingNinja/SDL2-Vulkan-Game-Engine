
/************************************************************************
*    FILE NAME:       visualcomponentscaledframe.h
*
*    DESCRIPTION:     Class for handling the visual part of a sprite
************************************************************************/

#ifndef __visual_component_scaledframe_h__
#define __visual_component_scaledframe_h__

// Physical component dependency
#include <2d/visualcomponentquad.h>

// Game lib dependencies
#include <common/rect.h>

class CVisualComponentScaledFrame : public CVisualComponentQuad
{
public:

    // Constructor
    CVisualComponentScaledFrame( const iObjectData & objectData );

    // Destructor
    virtual ~CVisualComponentScaledFrame();
    
    // Get the size
    const CSize<float> & getSize() const override;
    
private:
    
    // Update the UBO buffer
    void updateUBO(
        uint32_t index,
        CDevice & device,
        const iObjectVisualData & rVisualData,
        const CObjectTransform * const pObject,
        const CCamera & camera ) override;
};

#endif
