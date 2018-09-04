
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

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Vulkan lib dependencies
#include <system/vulkan.h>

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
        const CMatrix & model,
        const CMatrix & viewProj ) override;
    
    // Delete the assets that are otherwise freed when deleting the group
    // This is for when individual sprites are freed from the group
    void deleteGroupAssets() override;
    
private:
    
    // Reference to object visual data
    const CObjectData2D & m_rObjectData;
    
    // Command buffer
    std::vector<VkCommandBuffer> m_commandBufVec;
    
    // Descriptor Set for this image
    std::vector<VkDescriptorSet> m_descriptorSetVec;
    
    // Uniform buffers
    std::vector<CMemoryBuffer> m_uniformBufVec;

};

#endif  // __visual_component_quad_h__


