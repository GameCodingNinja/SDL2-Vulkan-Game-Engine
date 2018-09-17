
/************************************************************************
*    FILE NAME:       visualcomponentquad.h
*
*    DESCRIPTION:     Class for handling the visual part of 2d quad
************************************************************************/

#ifndef __visual_component_quad_h__
#define __visual_component_quad_h__

// Physical component dependency
#include <common/ivisualcomponent.h>

// Game lib dependencies
#include <utilities/matrix.h>
#include <common/color.h>
#include <common/size.h>
#include <common/pushdescriptorset.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Forward declaration(s)
class CObjectData2D;
class CMemoryBuffer;

class CVisualComponentQuad : public iVisualComponent, boost::noncopyable
{
public:

    // Constructor
    CVisualComponentQuad( const CObjectData2D & objectData );

    // Destructor
    virtual ~CVisualComponentQuad();
    
    // Record the command buffers
    void recordCommandBuffers( 
        uint32_t index,
        VkCommandBuffer cmdBuffer,
        const CMatrix & model,
        const CMatrix & viewProj ) override;
    
private:
    
    // Reference to object visual data
    const CObjectData2D & m_rObjectData;
    
    // Uniform buffers
    std::vector<CMemoryBuffer> m_uniformBufVec;
    
    // Push Descriptor set
    CPushDescriptorSet m_pushDescSet;

};

#endif  // __visual_component_quad_h__


