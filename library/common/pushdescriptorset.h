/************************************************************************
*    FILE NAME:       pushdescriptorset.h
*
*    DESCRIPTION:     push descriptor set
************************************************************************/  

#ifndef __push_descriptor_set_h__
#define __push_descriptor_set_h__

// Standard lib dependencies
#include <deque>

// Vulkan lib dependencies
#include <system/vulkan.h>

class CPushDescriptorSet
{
public:
    
    // Helper function for Push descriptor call
    void cmdPushDescriptorSet(
        uint32_t index,
        VkCommandBuffer cmdBuffer,
        VkPipelineLayout layout )
    {
        vkCmdPushDescriptorSetKHR( 
            cmdBuffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            layout,
            0,
            m_pushDescriptorSetVec[index].size(),
            m_pushDescriptorSetVec[index].data() );
    }
    
    // Push descriptor set function call
    PFN_vkCmdPushDescriptorSetKHR vkCmdPushDescriptorSetKHR;
    
    std::vector<std::vector<VkWriteDescriptorSet>> m_pushDescriptorSetVec;
    std::deque<VkDescriptorBufferInfo> m_descriptorBufferInfoDeq;
    std::deque<VkDescriptorImageInfo> m_descriptorImageInfoDeq;
};

#endif  // __push_descriptor_set_h__
