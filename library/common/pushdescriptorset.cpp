
/************************************************************************
*    FILE NAME:       pushdescriptorset.cpp
*
*    DESCRIPTION:     push descriptor set
************************************************************************/

// Physical component dependency
#include <common/pushdescriptorset.h>

// Game lib dependencies
#include <common/texture.h>

/************************************************************************
*    DESC:  Helper function for Push descriptor call
************************************************************************/
void CPushDescriptorSet::cmdPushDescriptorSet(
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


/************************************************************************
*    DESC:  Update the texture in the descriptor set
************************************************************************/
void CPushDescriptorSet::updateTexture( const CTexture & texture )
{
    m_descriptorImageInfoDeq.back().imageView = texture.m_textureImageView;
    m_descriptorImageInfoDeq.back().sampler = texture.m_textureSampler;
}