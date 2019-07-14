
/************************************************************************
*    FILE NAME:       pushdescriptorset.cpp
*
*    DESCRIPTION:     push descriptor set
************************************************************************/

// Physical component dependency
#include <system/pushdescriptorset.h>

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
    for( auto & iter : m_descriptorImageInfoDeq )
    {
        iter.imageView = texture.textureImageView;
        iter.sampler = texture.textureSampler;
    }
}
