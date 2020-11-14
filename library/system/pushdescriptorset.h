/************************************************************************
*    FILE NAME:       pushdescriptorset.h
*
*    DESCRIPTION:     push descriptor set
************************************************************************/  

#pragma once

// Standard lib dependencies
#include <deque>
#include <vector>

// Vulkan lib dependencies
#include <system/vulkan.h>

// Forward declaration(s)
class CTexture;

class CPushDescriptorSet
{
public:
    
    // Helper function for Push descriptor call
    void cmdPushDescriptorSet(
        uint32_t index,
        VkCommandBuffer cmdBuffer,
        VkPipelineLayout layout );
    
    // Update the texture in the descriptor set
    void updateTexture( const CTexture & texture );
    
    // Push descriptor set function call
    PFN_vkCmdPushDescriptorSetKHR vkCmdPushDescriptorSetKHR;
    
    std::vector<std::vector<VkWriteDescriptorSet>> m_pushDescriptorSetVec;
    std::deque<VkDescriptorBufferInfo> m_descriptorBufferInfoDeq;
    std::deque<VkDescriptorImageInfo> m_descriptorImageInfoDeq;
};
