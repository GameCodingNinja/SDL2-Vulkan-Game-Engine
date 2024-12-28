
/************************************************************************
*    FILE NAME:       visualcomponentquad.cpp
*
*    DESCRIPTION:     Class for handling the visual part of a sprite
************************************************************************/

// Physical component dependency
#include <2d/visualcomponentquad.h>

// Game lib dependencies
#include <objectdata/iobjectdata.h>
#include <objectdata/iobjectvisualdata.h>
#include <utilities/matrix.h>
#include <utilities/statcounter.h>
#include <common/camera.h>
#include <system/device.h>
#include <system/pipeline.h>
#include <system/uniformbufferobject.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CVisualComponentQuad::CVisualComponentQuad( const iObjectData & objectData ) :
    iVisualComponent( objectData ),
    m_rObjectData( objectData ),
    m_quadVertScale( objectData.getSize() * objectData.getVisualData().getDefaultUniformScale() ),
    m_pDescriptorSet(nullptr)
{
    auto & device( CDevice::Instance() );
    const uint32_t pipelineIndex( objectData.getVisualData().getPipelineIndex() );

    // Create the uniform buffer
    m_uniformBufVec = device.createUniformBufferVec( pipelineIndex );

    // Create the descriptor set
    if( GENERATION_TYPE != EGenType::FONT )
        m_pDescriptorSet = device.getDescriptorSet(
            pipelineIndex,
            objectData.getVisualData().getTexture(),
            m_uniformBufVec );
    
    // Create the push descriptor set
    // This is just data and doesn't need to be freed
    // if( GENERATION_TYPE != EGenType::FONT )
    //     device.createPushDescriptorSet(
    //         pipelineIndex,
    //         objectData.getVisualData().getTexture(),
    //         m_uniformBufVec,
    //         m_pushDescSet );
}

/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CVisualComponentQuad::~CVisualComponentQuad()
{
    CDevice::Instance().AddToDeleteQueue( m_uniformBufVec );
    CDevice::Instance().recycleDescriptorSet( m_pDescriptorSet );
}

/***************************************************************************
*   DESC:  Record the command buffers
****************************************************************************/
void CVisualComponentQuad::recordCommandBuffer(
    uint32_t index,
    VkCommandBuffer cmdBuffer,
    const CObject * const pObject,
    const CCamera & camera )
{
    if( allowCommandRecording() )
    {
        // Increment our stat counter to keep track of what is going on.
        CStatCounter::Instance().incDisplayCounter();
        
        const auto & rVisualData( m_rObjectData.getVisualData() );
        auto & device( CDevice::Instance() );

        // Get the pipeline data
        const SPipelineData & rPipelineData = device.getPipelineData( rVisualData.getPipelineIndex() );

        // Update the UBO buffer
        updateUBO( index, device, rVisualData, pObject, camera );

        // Bind the pipeline
        vkCmdBindPipeline( cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, rPipelineData.pipeline );

        // Bind vertex buffer
        VkBuffer vertexBuffers[] = {rVisualData.getVBO().m_buffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers( cmdBuffer, 0, 1, vertexBuffers, offsets );

        // Bind the index buffer
        vkCmdBindIndexBuffer( cmdBuffer, rVisualData.getIBO().m_buffer, 0, VK_INDEX_TYPE_UINT16 );

        // With the regular descriptor set implementation, objects that use the same texture and UBO can't share the same
        // descriptor set because the translation matrix is part of the UBO the objects sharing this will render ontop of each other
        vkCmdBindDescriptorSets( 
            cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, rPipelineData.pipelineLayout, 0, 1, &m_pDescriptorSet->m_descriptorVec[index], 0, nullptr );

        // Use the push descriptors
        //m_pushDescSet.cmdPushDescriptorSet( index, cmdBuffer, rPipelineData.pipelineLayout );

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
    const CObject * const pObject,
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
    const auto & rVisualData( m_rObjectData.getVisualData() );

    if( index < rVisualData.getFrameCount() )
    {
        iVisualComponent::setFrame( index );
        
        const auto & rTexture( rVisualData.getTexture( index ) );
        const float defScale( rVisualData.getDefaultUniformScale() );
        
        m_quadVertScale.w = rTexture.size.w * defScale;
        m_quadVertScale.h = rTexture.size.h * defScale;

        // Recycle if the descriptor set is not null
        // Can't update the descriptor set because it could be actuve in the command buffer.
        // The strategy is to recycle the current one and grab a fresh one
        if( m_pDescriptorSet != nullptr )
            CDevice::Instance().recycleDescriptorSet( m_pDescriptorSet );
            
        m_pDescriptorSet = CDevice::Instance().getDescriptorSet(
            m_rObjectData.getVisualData().getPipelineIndex(),
            rTexture,
            m_uniformBufVec );
        
        // Update the texture
        //m_pushDescSet.updateTexture( rTexture );
    }
}

/************************************************************************
*    DESC:  Is recording the command buffer allowed?
************************************************************************/
bool CVisualComponentQuad::allowCommandRecording()
{
    return ((GENERATION_TYPE > EGenType::_NULL_) && (GENERATION_TYPE < EGenType::FONT));
}

/************************************************************************
*    DESC:  Get the size
************************************************************************/
const CSize<float> & CVisualComponentQuad::getSize() const
{
    return m_quadVertScale;
}