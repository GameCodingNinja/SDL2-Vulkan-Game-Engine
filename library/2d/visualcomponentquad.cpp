
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
#include <utilities/exceptionhandling.h>
#include <common/quad2d.h>
#include <common/uniformbufferobject.h>
#include <common/pipeline.h>
#include <system/device.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CVisualComponentQuad::CVisualComponentQuad( const CObjectData2D & objectData ) :
    iVisualComponent( objectData.getVisualData() ),
    m_rObjectData( objectData )
{
    auto & device( CDevice::Instance() );
    const uint32_t pipelineIndex( objectData.getVisualData().getPipelineIndex() );

    // Create the uniform buffer
    m_uniformBufVec = device.createUniformBufferVec( pipelineIndex );
    
    // Create the push descriptor set
    // This is just data and doesn't need to be freed
    device.createPushDescriptorSet(
        pipelineIndex,
        objectData.getVisualData().getVulkanTexture(),
        m_uniformBufVec,
        m_pushDescSet );
}


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CVisualComponentQuad::~CVisualComponentQuad()
{
    if( !m_commandBufVec.empty() )
        CDevice::Instance().deleteCommandBuffer( m_rObjectData.getGroup(), m_commandBufVec );
    
    CDevice::Instance().deleteUniformBufferVec( m_uniformBufVec );
}


/***************************************************************************
*   DESC:  Record the command buffers
*          NOTE: this function is mainly for one off testing. Command buffers
*                should be created by the group and passed in normally
****************************************************************************/
void CVisualComponentQuad::recordCommandBuffers(
    uint32_t index,
    const CMatrix & model,
    const CMatrix & viewProj )
{
    auto & device( CDevice::Instance() );
    
    // Create the command buffer vector if it is empty
    if( m_commandBufVec.empty() )
        m_commandBufVec = device.createSecondaryCommandBuffers( objectData.getGroup() );
    
    // Setup to begine recording the command buffer
    VkCommandBufferInheritanceInfo cmdBufInheritanceInfo = {};
    cmdBufInheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
    cmdBufInheritanceInfo.framebuffer = device.getFrameBuffer( index );
    cmdBufInheritanceInfo.renderPass = device.getRenderPass();

    VkCommandBufferBeginInfo cmdBeginInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };  // VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
    cmdBeginInfo.flags =  VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT | VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
    cmdBeginInfo.pInheritanceInfo = &cmdBufInheritanceInfo;
    
    // Start recording the command buffer
    vkBeginCommandBuffer( m_commandBufVec[index], &cmdBeginInfo);
    
    // Record the command buffer
    recordCommandBuffers( index, m_commandBufVec[index], model, viewProj );
    
    // Stop recording the command buffer
    vkEndCommandBuffer( m_commandBufVec[index] );
    
    // Pass the command buffer to the queue
    device.updateCommandBuffer( m_commandBufVec[index] );
}

void CVisualComponentQuad::recordCommandBuffers(
    uint32_t index,
    VkCommandBuffer cmdBuffer,
    const CMatrix & model,
    const CMatrix & viewProj )
{
    const auto & rVisualData( m_rObjectData.getVisualData() );
    auto & device( CDevice::Instance() );
    
    //VkPipeline pipeline = device.getPipeline( rVisualData.getPipelineIndex() );
    const CPipelineData & rPipelineData = device.getPipelineData( rVisualData.getPipelineIndex() );
    
    // Setup the uniform buffer object
    NUBO::model_viewProj_color_additive ubo;
    ubo.model.setScale( rVisualData.getVertexScale() );
    ubo.model *= model;
    ubo.viewProj = viewProj;

    // Update the uniform buffer
    device.updateUniformBuffer( ubo, m_uniformBufVec[index].m_deviceMemory );
    
    vkCmdBindPipeline( cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, rPipelineData.m_pipeline );
    

    /*VkViewport viewport = {0, 0, 1280, 720, 0.0f, 1.0f};
    vkCmdSetViewport(m_commandBufVec[index], 0, 1, &viewport );
    
    VkRect2D scissor = {{50, 50}, {1000, 500}};
    vkCmdSetScissor( m_commandBufVec[index], 0, 1, &scissor );*/

    // Bind vertex buffer
    VkBuffer vertexBuffers[] = {rVisualData.getVBO().m_buffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers( cmdBuffer, 0, 1, vertexBuffers, offsets );

    // Bind the index buffer
    vkCmdBindIndexBuffer( cmdBuffer, rVisualData.getIBO().m_buffer, 0, VK_INDEX_TYPE_UINT16 );
    
    // Use the push descriptors
    m_pushDescSet.cmdPushDescriptorSet( index, cmdBuffer, rPipelineData.m_pipelineLayout );
    

    vkCmdDrawIndexed( cmdBuffer, rVisualData.getIBOCount(), 1, 0, 0, 0 );
}