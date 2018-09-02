
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
#include <system/device.h>


struct UniformBufferObject {
    CMatrix cameraViewMatrix;
    CMatrix projectionMatrix;
    CColor color;
    CColor additive;
    int renderType;
};


/************************************************************************
*    desc:  Constructor
************************************************************************/
CVisualComponentQuad::CVisualComponentQuad( const CObjectData2D & objectData ) :
    iVisualComponent( objectData.getVisualData() ),
    m_rVisualData( objectData.getVisualData() )
{
    // Create the uniform buffer
    m_uniformBufVec = CDevice::Instance().createUniformBuffer( sizeof(UniformBufferObject) );
    
    // Create the descriptor set
    m_descriptorSetVec = CDevice::Instance().createDescriptorSet(
        objectData.getGroup(),
        objectData.getVisualData().getVulkanTexture(),
        m_uniformBufVec,
        sizeof(UniformBufferObject) );
    
    // Create the command buffer vector
    m_commandBufVec = CDevice::Instance().createSecondaryCommandBuffers( objectData.getGroup() );
    
}   // constructor


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CVisualComponentQuad::~CVisualComponentQuad()
{
    CDevice::Instance().freeMemoryBuffer( m_uniformBufVec );
    
}   // destructor


/***************************************************************************
*   DESC:  Record the command buffers
****************************************************************************/
void CVisualComponentQuad::recordCommandBuffers( uint32_t cmdBufIndex )
{
    UniformBufferObject ubo;

    ubo.projectionMatrix.orthographicRH(
        CSettings::Instance().getDefaultSize().w,
        CSettings::Instance().getDefaultSize().h,
        CSettings::Instance().getMinZdist(),
        CSettings::Instance().getMaxZdist() );

    CMatrix objMatrix;
    objMatrix.translate( CPoint<float>(0.f, 0.f, -10.0f) );

    ubo.cameraViewMatrix.setScale( m_rVisualData.getVertexScale() );
    ubo.cameraViewMatrix *= objMatrix;
    
    // Update the uniform buffer
    CDevice::Instance().updateUniformBuffer( ubo, m_uniformBufVec[cmdBufIndex].m_deviceMemory );

    VkCommandBufferInheritanceInfo cmdBufInheritanceInfo = {};
    cmdBufInheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
    cmdBufInheritanceInfo.framebuffer = CDevice::Instance().getFrameBuffer( cmdBufIndex );
    cmdBufInheritanceInfo.renderPass = CDevice::Instance().getRenderPass();

    VkCommandBufferBeginInfo cmdBeginInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };  // VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
    cmdBeginInfo.flags =  VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT | VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
    cmdBeginInfo.pInheritanceInfo = &cmdBufInheritanceInfo;

    vkBeginCommandBuffer( m_commandBufVec[cmdBufIndex], &cmdBeginInfo);
    vkCmdBindPipeline( m_commandBufVec[cmdBufIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, CDevice::Instance().getGraphicsPipeline());

    // Bind vertex buffer
    VkBuffer vertexBuffers[] = {m_rVisualData.getVBO().m_buffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers( m_commandBufVec[cmdBufIndex], 0, 1, vertexBuffers, offsets );

    // Bind the index buffer
    vkCmdBindIndexBuffer( m_commandBufVec[cmdBufIndex], m_rVisualData.getIBO().m_buffer, 0, VK_INDEX_TYPE_UINT16 );

    vkCmdBindDescriptorSets( m_commandBufVec[cmdBufIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, CDevice::Instance().getPipelinelayout(), 0, 1, &m_descriptorSetVec[cmdBufIndex], 0, nullptr);

    vkCmdDrawIndexed( m_commandBufVec[cmdBufIndex], m_rVisualData.getIBOCount(), 1, 0, 0, 0 );

    vkEndCommandBuffer( m_commandBufVec[cmdBufIndex] );
    
    
    CDevice::Instance().updateCommandBuffer( m_commandBufVec[cmdBufIndex] );
}
