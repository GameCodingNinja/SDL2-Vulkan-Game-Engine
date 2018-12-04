
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
#include <utilities/matrix.h>
#include <utilities/statcounter.h>
#include <common/uniformbufferobject.h>
#include <common/pipeline.h>
#include <common/camera.h>
#include <system/device.h>
#include <2d/object2d.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CVisualComponentQuad::CVisualComponentQuad( const CObjectData2D & objectData ) :
    iVisualComponent( objectData ),
    m_rObjectData( objectData ),
    m_quadVertScale( objectData.getSize() * objectData.getVisualData().getDefaultUniformScale() )
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
}


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CVisualComponentQuad::~CVisualComponentQuad()
{
    CDevice::Instance().AddToDeleteQueue( m_uniformBufVec );
}


/***************************************************************************
*   DESC:  Record the command buffers
****************************************************************************/
void CVisualComponentQuad::recordCommandBuffer(
    uint32_t index,
    VkCommandBuffer cmdBuffer,
    const CObject2D * const pObject,
    const CCamera & camera )
{
    if( allowCommandRecording() )
    {
        // Increment our stat counter to keep track of what is going on.
        CStatCounter::Instance().incDisplayCounter();
        
        const auto & rVisualData( m_rObjectData.getVisualData() );
        auto & device( CDevice::Instance() );

        // Get the pipeline data
        const CPipelineData & rPipelineData = device.getPipelineData( rVisualData.getPipelineIndex() );

        // Update the UBO buffer
        updateUBO( index, device, rVisualData, pObject, camera );

        // Bind the pipeline
        vkCmdBindPipeline( cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, rPipelineData.m_pipeline );

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
}


/************************************************************************
*    DESC:  Update the UBO buffer
************************************************************************/
void CVisualComponentQuad::updateUBO(
    uint32_t index,
    CDevice & device,
    const iObjectVisualData & rVisualData,
    const CObject2D * const pObject,
    const CCamera & camera )
{
    // Setup the uniform buffer object
    NUBO::model_viewProj_color_additive ubo;
    ubo.model.setScale( m_quadVertScale );
    ubo.model *= pObject->getMatrix();
    ubo.viewProj = camera.getFinalMatrix();
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
    
    const auto & rVisualData( m_rObjectData.getVisualData() );
    const auto & rTexture( rVisualData.getTexture( index ) );
    const float defScale( rVisualData.getDefaultUniformScale() );
    
    m_quadVertScale.w = rTexture.size.w * defScale;
    m_quadVertScale.h = rTexture.size.h * defScale;
    
    // Update the texture
    m_pushDescSet.updateTexture( rTexture );
}


/************************************************************************
*    DESC:  Is recording the command buffer allowed?
************************************************************************/
bool CVisualComponentQuad::allowCommandRecording()
{
    return ((GENERATION_TYPE > NDefs::EGT_NULL) && (GENERATION_TYPE < NDefs::EGT_FONT));
}


/************************************************************************
*    DESC:  Get the size
************************************************************************/
const CSize<float> & CVisualComponentQuad::getSize() const
{
    return m_quadVertScale;
}