
/************************************************************************
*    FILE NAME:       visualcomponentscaledframe.h
*
*    DESCRIPTION:     Class for handling the visual part of a sprite
************************************************************************/

#pragma once

// Physical component dependency
#include <2d/visualcomponentquad.h>

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
        const CObject * const pObject,
        const CCamera & camera ) override;
};
