
/************************************************************************
*    FILE NAME:       visualcomponentquad.cpp
*
*    DESCRIPTION:     Class for handling the visual part of 2d quad
************************************************************************/

// Physical component dependency
#include <2d/visualcomponentquad.h>

// Game lib dependencies
#include <objectdata/objectdata2d.h>
#include <objectdata/objectvisualdata2d.h>
#include <utilities/settings.h>
#include <common/quad2d.h>
#include <common/uniformbufferobject.h>
#include <system/device.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CVisualComponentQuad::CVisualComponentQuad( const CObjectData2D & objectData ) :
    iVisualComponent( objectData.getVisualData() ),
    m_rObjectData( objectData )
{
    auto & device( CDevice::Instance() );
    
    // Create the uniform buffer
    m_uniformBufVec = device.createUniformBuffer( sizeof(UniformBufferObject) );
    
    // Create the descriptor set
    m_descriptorSetVec = device.createDescriptorSet(
        objectData.getGroup(),
        objectData.getVisualData().getVulkanTexture(),
        m_uniformBufVec,
        sizeof(UniformBufferObject) );
    
    // Create the command buffer vector
    m_commandBufVec = device.createSecondaryCommandBuffers( objectData.getGroup() );
}


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CVisualComponentQuad::~CVisualComponentQuad()
{
    CDevice::Instance().deleteMemoryBuffer( m_uniformBufVec );
}


/***************************************************************************
*   DESC:  Delete the assets that are otherwise freed when deleting the group
*          This is for when individual sprites are freed from the group
****************************************************************************/
void CVisualComponentQuad::deleteGroupAssets()
{
    CDevice::Instance().deleteCommandBuffer( m_rObjectData.getGroup(), m_commandBufVec );
    CDevice::Instance().deleteDescriptorSet( m_rObjectData.getGroup(), m_descriptorSetVec );
}


/***************************************************************************
*   DESC:  Record the command buffers
****************************************************************************/
void CVisualComponentQuad::recordCommandBuffers(
    uint32_t cmdBufIndex,
    const CMatrix & model,
    const CMatrix & viewProj )
{
    const auto & rVisualData( m_rObjectData.getVisualData() );
    auto & device( CDevice::Instance() );
    
    // Setup the uniform buffer object
    UniformBufferObject ubo;
    ubo.model.setScale( rVisualData.getVertexScale() );
    ubo.model *= model;
    ubo.viewProj = viewProj;

    // Update the uniform buffer
    device.updateUniformBuffer( ubo, m_uniformBufVec[cmdBufIndex].m_deviceMemory );

    VkCommandBufferInheritanceInfo cmdBufInheritanceInfo = {};
    cmdBufInheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
    cmdBufInheritanceInfo.framebuffer = device.getFrameBuffer( cmdBufIndex );
    cmdBufInheritanceInfo.renderPass = device.getRenderPass();

    VkCommandBufferBeginInfo cmdBeginInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };  // VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
    cmdBeginInfo.flags =  VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT | VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
    cmdBeginInfo.pInheritanceInfo = &cmdBufInheritanceInfo;

    vkBeginCommandBuffer( m_commandBufVec[cmdBufIndex], &cmdBeginInfo);
    vkCmdBindPipeline( m_commandBufVec[cmdBufIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, device.getGraphicsPipeline());

    // Bind vertex buffer
    VkBuffer vertexBuffers[] = {rVisualData.getVBO().m_buffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers( m_commandBufVec[cmdBufIndex], 0, 1, vertexBuffers, offsets );

    // Bind the index buffer
    vkCmdBindIndexBuffer( m_commandBufVec[cmdBufIndex], rVisualData.getIBO().m_buffer, 0, VK_INDEX_TYPE_UINT16 );

    vkCmdBindDescriptorSets( m_commandBufVec[cmdBufIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, device.getPipelinelayout(), 0, 1, &m_descriptorSetVec[cmdBufIndex], 0, nullptr);

    vkCmdDrawIndexed( m_commandBufVec[cmdBufIndex], rVisualData.getIBOCount(), 1, 0, 0, 0 );

    vkEndCommandBuffer( m_commandBufVec[cmdBufIndex] );
    
    
    device.updateCommandBuffer( m_commandBufVec[cmdBufIndex] );
}
