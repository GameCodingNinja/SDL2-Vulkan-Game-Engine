
/************************************************************************
*    FILE NAME:       visualcomponentquad.h
*
*    DESCRIPTION:     Class for handling the visual part of a sprite
************************************************************************/

#ifndef __visual_component_quad_h__
#define __visual_component_quad_h__

// Physical component dependency
#include <common/ivisualcomponent.h>

// Game lib dependencies
#include <common/pushdescriptorset.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Forward declaration(s)
class CObjectData2D;
class iObjectVisualData;
class CMemoryBuffer;
class CDevice;

class CVisualComponentQuad : public iVisualComponent, boost::noncopyable
{
public:

    // Constructor
    CVisualComponentQuad( const CObjectData2D & objectData );

    // Destructor
    virtual ~CVisualComponentQuad();
    
    // Record the command buffers
    virtual void recordCommandBuffer( 
        uint32_t index,
        VkCommandBuffer cmdBuffer,
        const CObject2D * const pObject,
        const CCamera & camera ) override;
    
    // Set the frame index
    virtual void setFrame( uint index ) override;
    
protected:
    
    // Is recording the command buffer allowed?
    virtual bool allowCommandRecording();
    
private:
    
    // Update the UBO buffer
    virtual void updateUBO(
        uint32_t index,
        CDevice & device,
        const iObjectVisualData & rVisualData,
        const CObject2D * const pObject,
        const CCamera & camera );

protected:
    
    // Uniform buffers
    std::vector<CMemoryBuffer> m_uniformBufVec;
    
    // Reference to object visual data
    const CObjectData2D & m_rObjectData;

    // Push Descriptor set
    CPushDescriptorSet m_pushDescSet;
};

#endif
