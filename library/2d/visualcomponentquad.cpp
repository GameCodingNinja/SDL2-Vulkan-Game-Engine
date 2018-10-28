
/************************************************************************
*    FILE NAME:       visualcomponentquad.cpp
*
*    DESCRIPTION:     Class for handling the visual part of a sprite
************************************************************************/

// Physical component dependency
#include <2d/visualcomponentquad.h>

// Game lib dependencies
#include <objectdata/objectdata2d.h>
#include <objectdata/iobjectvisualdata.h>
#include <utilities/settings.h>
#include <utilities/exceptionhandling.h>
#include <utilities/matrix.h>
#include <common/uniformbufferobject.h>
#include <common/pipeline.h>
#include <system/device.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CVisualComponentQuad::CVisualComponentQuad( const CObjectData2D & objectData ) :
    iVisualComponent( objectData ),
    m_rObjectData( objectData )
{
    auto & device( CDevice::Instance() );
    const uint32_t pipelineIndex( objectData.getVisualData().getPipelineIndex() );

    // Create the uniform buffer
    m_uniformBufVec = device.createUniformBufferVec( pipelineIndex );
    
    // Create the push descriptor set
    // This is just data and doesn't need to be freed
    if( GENERATION_TYPE != NDefs::EGT_FONT )
        device.createPushDescriptorSet(
            pipelineIndex,
            objectData.getVisualData().getTexture(),
            m_uniformBufVec,
            m_pushDescSet );
    
    //m_commandBufVec = CDevice::Instance().createSecondaryCommandBuffers( "(startup)" );
}


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CVisualComponentQuad::~CVisualComponentQuad()
{
    CDevice::Instance().deleteUniformBufferVec( m_uniformBufVec );
    //CDevice::Instance().deleteCommandBuffer( m_rObjectData.getGroup(), m_commandBufVec );
}


/***************************************************************************
*   DESC:  Record the command buffers
****************************************************************************/
void CVisualComponentQuad::recordCommandBuffer(
    uint32_t index,
    VkCommandBuffer cmdBuffer,
    const CMatrix & model,
    const CMatrix & viewProj )
{
    if( allowCommandRecording() )
    {
        const auto & rVisualData( m_rObjectData.getVisualData() );
        auto & device( CDevice::Instance() );

        // Get the pipeline data
        const CPipelineData & rPipelineData = device.getPipelineData( rVisualData.getPipelineIndex() );

        // Update the UBO buffer
        updateUBO( index, device, rVisualData, model, viewProj );

        //CDevice::Instance().beginCommandBuffer( index, m_commandBufVec[index] );

        // Bind the pipeline
        if( device.getLastPipeline() != rPipelineData.m_pipeline )
        {
            vkCmdBindPipeline( cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, rPipelineData.m_pipeline );
            device.setLastPipeline( rPipelineData.m_pipeline );
        }

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

        // Do the draw
        vkCmdDrawIndexed( cmdBuffer, rVisualData.getIBOCount(), 1, 0, 0, 0 );
    }
    
    //CDevice::Instance().endCommandBuffer( m_commandBufVec[index] );
}


/************************************************************************
*    DESC:  Update the UBO buffer
************************************************************************/
void CVisualComponentQuad::updateUBO(
    uint32_t index,
    CDevice & device,
    const iObjectVisualData & rVisualData,
    const CMatrix & model,
    const CMatrix & viewProj )
{
    // Setup the uniform buffer object
    NUBO::model_viewProj_color_additive ubo;
    ubo.model.setScale( rVisualData.getVertexScale() );
    ubo.model *= model;
    ubo.viewProj = viewProj;
    ubo.color = m_color;
    ubo.additive = m_additive;

    // Update the uniform buffer
    device.updateUniformBuffer( ubo, m_uniformBufVec[index].m_deviceMemory );
}


/************************************************************************
*    DESC:  Set the frame ID from index
************************************************************************/
void CVisualComponentQuad::setFrame( uint index )
{
    iVisualComponent::setFrame( index );
    
    // Update the texture
    m_pushDescSet.updateTexture( m_rObjectData.getVisualData().getTexture( index ) );
}


/************************************************************************
*    DESC:  Is recording the command buffer allowed?
************************************************************************/
bool CVisualComponentQuad::allowCommandRecording()
{
    return ((GENERATION_TYPE > NDefs::EGT_NULL) && (GENERATION_TYPE < NDefs::EGT_FONT));
}
