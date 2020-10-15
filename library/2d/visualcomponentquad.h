
/************************************************************************
*    FILE NAME:       visualcomponentquad.h
*
*    DESCRIPTION:     Class for handling the visual part of a sprite
************************************************************************/

#pragma once

// Physical component dependency
#include <common/ivisualcomponent.h>

// Game lib dependencies
//#include <system/pushdescriptorset.h>
#include <system/descriptorset.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Forward declaration(s)
class iObjectVisualData;
class CMemoryBuffer;
class CDevice;

class CVisualComponentQuad : public iVisualComponent, boost::noncopyable
{
public:

    // Constructor
    CVisualComponentQuad( const iObjectData & objectData );

    // Destructor
    virtual ~CVisualComponentQuad();
    
    // Record the command buffers
    virtual void recordCommandBuffer( 
        uint32_t index,
        VkCommandBuffer cmdBuffer,
        const CObject * const pObject,
        const CCamera & camera ) override;
    
    // Set the frame index
    virtual void setFrame( uint index ) override;
    
    // Get the size
    virtual const CSize<float> & getSize() const override;
    
protected:
    
    // Is recording the command buffer allowed?
    virtual bool allowCommandRecording();
    
private:
    
    // Update the UBO buffer
    virtual void updateUBO(
        uint32_t index,
        CDevice & device,
        const iObjectVisualData & rVisualData,
        const CObject * const pObject,
        const CCamera & camera );

protected:
    
    // Uniform buffers
    std::vector<CMemoryBuffer> m_uniformBufVec;
    
    // Reference to object visual data
    const iObjectData & m_rObjectData;
    
    // The scale of the quad
    CSize<float> m_quadVertScale;

    // Descriptor Set for this image
    CDescriptorSet * m_pDescriptorSet;

    // Push Descriptor set
    //CPushDescriptorSet m_pushDescSet;
};
